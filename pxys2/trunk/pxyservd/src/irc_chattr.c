#include <limits.h>
const char ToLowerTab_8859_1[] = {
#if (CHAR_MIN<0)
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xc8-xcf */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xd0-xd7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xd7',
/* xd8-xdf */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xdf',
/* xe0-xe7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xe8-xef */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xf0-xf7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
/* xf8-xff */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff'
                ,
#endif /* (CHAR_MIN<0) */
/* x00-x07 */ '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
/* x08-x0f */ '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
/* x10-x17 */ '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
/* x18-x1f */ '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
/* ' '-x27 */    ' ',    '!',    '"',    '#',    '$',    '%',    '&', '\x27',
/* '('-'/' */    '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
/* '0'-'7' */    '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
/* '8'-'?' */    '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
/* '@'-'G' */    '@',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
/* 'H'-'O' */    'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
/* 'P'-'W' */    'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
/* 'X'-'_' */    'x',    'y',    'z',    '{',    '|',    '}',    '~',    '_',
/* '`'-'g' */    '`',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
/* 'h'-'o' */    'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
/* 'p'-'w' */    'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
/* 'x'-x7f */    'x',    'y',    'z',    '{',    '|',    '}',    '~', '\x7f'
#if (!(CHAR_MIN<0))
                ,
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xc8-xcf */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xd0-xd7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xd7',
/* xd8-xdf */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xdf',
/* xe0-xe7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xe8-xef */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xf0-xf7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
/* xf8-xff */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff'
#endif /* (!(CHAR_MIN<0)) */
  };

const char ToUpperTab_8859_1[] = {
#if (CHAR_MIN<0)
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xc8-xcf */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xd0-xd7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
/* xd8-xdf */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
/* xe0-xe7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xe8-xef */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xf0-xf7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xf7',
/* xf8-xff */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xff'
                ,
#endif /* (CHAR_MIN<0) */
/* x00-x07 */ '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
/* x08-x0f */ '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
/* x10-x17 */ '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
/* x18-x1f */ '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
/* ' '-x27 */    ' ',    '!',    '"',    '#',    '$',    '%',    '&', '\x27',
/* '('-'/' */    '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
/* '0'-'7' */    '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
/* '8'-'?' */    '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
/* '@'-'G' */    '@',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
/* 'H'-'O' */    'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
/* 'P'-'W' */    'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
/* 'X'-'_' */    'X',    'Y',    'Z',    '[', '\x5c',    ']',    '^',    '_',
/* '`'-'g' */    '`',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
/* 'h'-'o' */    'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
/* 'p'-'w' */    'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
/* 'x'-x7f */    'X',    'Y',    'Z',    '[', '\x5c',    ']',    '^', '\x7f'
#if (!(CHAR_MIN<0))
                ,
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xc8-xcf */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xd0-xd7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
/* xd8-xdf */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
/* xe0-xe7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xe8-xef */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xf0-xf7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xf7',
/* xf8-xff */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xff'
#endif /* (!(CHAR_MIN<0)) */
  };

const unsigned int IRCD_CharAttrTab[] = {
#if (CHAR_MIN<0)
/* x80-x87 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x88-x8f */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x90-x97 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x98-x9f */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xa0-xa7 */ 0x0000, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xa8-xaf */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xb0-xb7 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xb8-xbf */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xc0-xc7 */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00,
/* xc8-xcf */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00,
/* xd0-xd7 */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x0400,
/* xd8-xdf */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x0400,
/* xe0-xe7 */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400,
/* xe8-xef */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400,
/* xf0-xf7 */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x0400,
/* xf8-xff */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x0400
                ,
#endif /* (CHAR_MIN<0) */
/* x00-x07 */ 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
/* x08-x0f */ 0x0004, 0x0104, 0x10104, 0x0104, 0x0104, 0x10104, 0x0004, 0x0004,
/* x10-x17 */ 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
/* x18-x1f */ 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004,
/* ' '-x27 */ 0x20140, 0x04d0, 0x04d0, 0x404d0, 0x04d0, 0x04d0, 0x404d0, 0x24d0,
/* '('-'/' */ 0x04d0, 0x04d0, 0x04d0, 0x04d0, 0x200d0, 0x274d0, 0xe4d0, 0x04d0,
/* '0'-'7' */ 0x2f459, 0x2f459, 0x2f459, 0x2f459, 0x2f459, 0x2f459, 0x2f459, 0x2f459,
/* '8'-'?' */ 0x2f459, 0x2f459, 0x04d0, 0x04d0, 0x04d0, 0x04d0, 0x04d0, 0x04d0,
/* '@'-'G' */ 0x04d0, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653,
/* 'H'-'O' */ 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653,
/* 'P'-'W' */ 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653,
/* 'X'-'_' */ 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x7653, 0x74d0,
/* '`'-'g' */ 0x34d0, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473,
/* 'h'-'o' */ 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473,
/* 'p'-'w' */ 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473,
/* 'x'-x7f */ 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x7473, 0x0400
#if (!(CHAR_MIN<0))
                ,
/* x80-x87 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x88-x8f */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x90-x97 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* x98-x9f */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xa0-xa7 */ 0x0000, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xa8-xaf */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xb0-xb7 */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xb8-xbf */ 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400,
/* xc0-xc7 */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00,
/* xc8-xcf */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00,
/* xd0-xd7 */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x0400,
/* xd8-xdf */ 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x2c00, 0x0400,
/* xe0-xe7 */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400,
/* xe8-xef */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400,
/* xf0-xf7 */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x0400,
/* xf8-xff */ 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x2400, 0x0400
#endif /* (!(CHAR_MIN<0)) */
  };

