// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed
// DAI-Labor, TU-Berlin
//
// This file is part of libSML.
//
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SML_BOOLEAN_H_
#define SML_BOOLEAN_H_

#define SML_BOOLEAN_TRUE 0xFF
#define SML_BOOLEAN_FALSE 0x00

#include "sml_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 sml_boolean;

sml_boolean *sml_boolean_init(u8 b);
sml_boolean *sml_boolean_parse(sml_buffer *buf);
void sml_boolean_write(sml_boolean *boolean, sml_buffer *buf);
void sml_boolean_free(sml_boolean *b);

#ifdef __cplusplus
}
#endif

#endif /* SML_BOOLEAN_H_ */
