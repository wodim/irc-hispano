/*
 * IRC - Internet Relay Chat, ircd/m_config.c
 * Copyright (C) 1999 IRC-Hispano.org - ESNET - jcea & savage
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include "config.h"

#if defined(ESNET_NEG)

#include "h.h"
#include "client.h"
#include "s_debug.h"
#include "common.h"
#include "ircd.h"
#include "struct.h"
#include "s_serv.h"
#include "msg.h"
#include "ircd_alloc.h"
#include "s_conf.h"
#include "m_config.h"
#include "send.h"
#include "numnicks.h"
#include "s_bdd.h"

#include <assert.h>

typedef enum { FIN, ZLIB, TOK } opcion;

static struct {
  char *texto;
  opcion opcion;
}
opciones[] = {
#if defined(ZLIB_ESNET)
  {"zlib", ZLIB},
#endif
  {"tok", TOK},
  {NULL, FIN}
};

#if defined(ZLIB_ESNET)
voidpf z_alloc(voidpf opaque, uInt items, uInt size)
{
  return MyCalloc(items, size);
}

void z_free(voidpf opaque, voidpf address)
{
  MyFree(address);
}

#endif

typedef enum { YO2EL, EL2YO } tipo_config;

static char *mira_conf_negociacion(char *nombre, tipo_config tipo_config)
{
  aConfItem *p;

  for (p = conf_negociacion; p; p = p->next)
  {
    if (!ircd_strcmp(nombre, p->name))
      break;
  }
  if (!p)
    return "";
  if (tipo_config == YO2EL)
    return p->host;
  return p->passwd;
}

void config_resolve_speculative(struct Client *cptr)
{
  char *p;
  int estado;

  p = mira_conf_negociacion(cptr->name, YO2EL);

  if (IsServer(cptr) || (IsCookieEncrypted(cptr) && compresion_zlib_cliente))
  {
#if defined(ZLIB_ESNET)
    if (!strchr(p, 'z') && (cptr->cli_connect->negociacion & ZLIB_ESNET_OUT_SPECULATIVE))
    {
#if !defined(NO_PROTOCOL9)
      if (!IsServer(cptr) || Protocol(cptr) < 10)
        sendto_one(cptr, ":%s " MSG_CONFIG " ACK :zlib", me.name);
      else
#endif
        sendto_one(cptr, "%s " TOK_CONFIG " ACK :zlib", NumServ(&me));
      cptr->cli_connect->negociacion &= ~ZLIB_ESNET_OUT_SPECULATIVE;
      cptr->cli_connect->negociacion |= ZLIB_ESNET_OUT;
      cptr->cli_connect->comp_out = MyMalloc(sizeof(z_stream));
      if (!cptr->cli_connect->comp_out)
        outofmemory();
      cptr->cli_connect->comp_out->zalloc = z_alloc;
      cptr->cli_connect->comp_out->zfree = z_free;
      cptr->cli_connect->comp_out->opaque = 0;
      estado = deflateInit(cptr->cli_connect->comp_out, IsServer(cptr) ? 9 : compresion_zlib_cliente);
      assert(estado == Z_OK);
      cptr->cli_connect->comp_out_total_in = 0;
      cptr->cli_connect->comp_out_total_out = 0;
    }
#endif
  }                             /* No es server */
}

int config_ack(struct Client *cptr, struct Client *sptr, char *fuente, char *valor,
    opcion opcion)
{
  int estado;

  if (IsServer(cptr))
  {
    switch (opcion)
    {
#if defined(ZLIB_ESNET)
      case ZLIB:
        cptr->cli_connect->negociacion |= ZLIB_ESNET_IN;
        cptr->cli_connect->comp_in = MyMalloc(sizeof(z_stream));
        if (!cptr->cli_connect->comp_in)
          outofmemory();
        cptr->cli_connect->comp_in->next_in = Z_NULL;
        cptr->cli_connect->comp_in->avail_in = 0;
        cptr->cli_connect->comp_in->zalloc = z_alloc;
        cptr->cli_connect->comp_in->zfree = z_free;
        cptr->cli_connect->comp_in->opaque = 0;
        estado = inflateInit(cptr->cli_connect->comp_in);
        assert(estado == Z_OK);
        cptr->cli_connect->comp_in_total_in = 0;
        cptr->cli_connect->comp_in_total_out = 0;
        break;
#endif
      default:                 /* No deberia ocurrir nunca */
        break;
    }
  }
  return 0;
}

int config_req(struct Client *cptr, struct Client *sptr, char *fuente, char *valor,
    opcion opcion)
{
  char *p;

  p = mira_conf_negociacion(PunteroACadena(sptr->name), YO2EL);

  if (IsServer(sptr))
  {
    switch (opcion)
    {
#if defined(ZLIB_ESNET)
      case ZLIB:
        if (!strchr(p, 'z'))
        {                       /* Permitimos compresion */
#if !defined(NO_PROTOCOL9)
          if (Protocol(sptr) < 10)
            sendto_one(sptr, ":%s " MSG_CONFIG " ACK :zlib", me.name);
          else
#endif
            sendto_one(sptr, "%s " TOK_CONFIG " ACK :zlib", NumServ(&me));
          cptr->cli_connect->negociacion |= ZLIB_ESNET_OUT;
        }
        break;
#endif
      case FIN:
        assert(0);              /* No deberia darse nunca */
    }
  }
  else
  {                             /* No es servidor o no lo sabemos aun */
    switch (opcion)
    {
#if defined(ZLIB_ESNET)
      case ZLIB:
        cptr->cli_connect->negociacion |= ZLIB_ESNET_OUT_SPECULATIVE;
        break;
#endif
      case TOK:
          if(IsCookieEncrypted(cptr)) {
            sendto_one(sptr, ":%s " MSG_CONFIG " ACK :tok", me.name);
            cptr->cli_connect->negociacion |= USER_TOK;
          }
          break;
      case FIN:
        assert(0);              /* No deberia darse nunca */
    }
  }
  return 0;
}

int m_config(struct Client *cptr, struct Client *sptr, int parc, char *parv[])
{
  int i;

  if (!MyConnect(cptr) || (cptr != sptr))
  {
    sendto_one(sptr, ":%s NOTICE %s :Solo conexiones locales", me.name,
        parv[0]);
    return 0;
  }
  if (parc < 3)
  {
    return 0;
  }

  for (i = 0; opciones[i].texto != NULL; i++)
  {
    if (!strcasecmp(opciones[i].texto, parv[2]))
      break;
  }
  if (opciones[i].texto == NULL)
  {
    sendto_one(sptr, ":%s NOTICE %s :Valor negociado desconocido", me.name,
        PunteroACadena(sptr->name));
    return 0;
  }

  if (!strcasecmp("ACK", parv[1]))
    return config_ack(cptr, sptr, parv[0], parv[2], opciones[i].opcion);
  if (!strcasecmp("REQ", parv[1]))
    return config_req(cptr, sptr, parv[0], parv[2], opciones[i].opcion);
  
  return 0;
}

void envia_config_req(struct Client *cptr)
{
  char *p;

  p = mira_conf_negociacion(cptr->name, EL2YO);
#if defined(ZLIB_ESNET)
  if (!strchr(p, 'z'))          /* Queremos que nos mande comprimido */
    sendto_one(cptr, ":%s " MSG_CONFIG " REQ :zlib", me.name);
#endif
}


#endif /* M_CONFIG.C */

