AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = pxyscand

pxyscand_SOURCES = \
	PXConfig.h PXConfigLoader.h PXCore.h PXIPCacheT.h PXMCrazyBandit.h PXMHttpProxy.h PXMHttpGet.h PXMSocks.h PXMWingate.h PXMutex.h PXRepeater.h PXScan.h PXScanManager.h PXScanModule.h PXServer.h PXSession.h PXXMLXPathLoaderImp.h socks4.h socks5.h sha256.h PXSecret.h \
	pxyscand.cc PXCore.cc PXConfig.cc PXConfigLoader.cc PXXMLXPathLoaderImp.cc PXMutex.cc PXRepeater.cc PXServer.cc PXScan.cc PXScanManager.cc PXScanModule.cc PXSession.cc sha256.c PXSecret.c
pxyscand_SOURCES += PXMCrazyBandit.cc PXMHttpProxy.cc PXMHttpGet.cc PXMSocks.cc PXMWingate.cc
pxyscand_SOURCES += PXIPCacheT.cc
pxyscand_SOURCES += ../../ipcache/ipcache.c

pxyscand_LDADD = ../../libopas/opas/.libs/libopas.a

INCLUDES = -Wall -I../include
