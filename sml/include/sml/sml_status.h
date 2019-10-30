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

#ifndef SML_STATUS_H_
#define SML_STATUS_H_

#include "sml_number.h"
#include "sml_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u8 type;
	union {
		u8 *status8;
		u16 *status16;
		u32 *status32;
		u64 *status64;
	} data;
} sml_status;

sml_status *sml_status_init();
sml_status *sml_status_parse(sml_buffer *buf);
void sml_status_write(sml_status *status, sml_buffer *buf);
void sml_status_free(sml_status *status);

#ifdef __cplusplus
}
#endif

#endif /* SML_STATUS_H_ */
