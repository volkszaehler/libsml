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

#ifndef SML_TIME_H_
#define SML_TIME_H_

#include "sml_number.h"
#include "sml_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SML_TIME_SEC_INDEX 0x01
#define SML_TIME_TIMESTAMP 0x02

typedef struct {
	u8 *tag;
	union {
		u32 *sec_index;
		u32 *timestamp;
	} data;
} sml_time;

sml_time *sml_time_init();
sml_time *sml_time_parse(sml_buffer *buf);
void sml_time_write(sml_time *time, sml_buffer *buf);
void sml_time_free(sml_time *time);
#ifdef __cplusplus
}
#endif

#endif /* SML_TIME_H_ */
