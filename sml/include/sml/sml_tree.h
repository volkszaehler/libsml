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

#ifndef SML_TREE_H_
#define SML_TREE_H_

#include "sml_octet_string.h"
#include "sml_shared.h"
#include "sml_time.h"
#include "sml_value.h"

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
#define SML_PROC_PAR_VALUE_TAG_VALUE        0x01
#define SML_PROC_PAR_VALUE_TAG_PERIOD_ENTRY 0x02
#define SML_PROC_PAR_VALUE_TAG_TUPEL_ENTRY  0x03
#define SML_PROC_PAR_VALUE_TAG_TIME         0x04
// clang-format on

// what a messy tupel ...
typedef struct {
	octet_string *server_id;
	sml_time *sec_index;
	u64 *status;

	sml_unit *unit_pA;
	i8 *scaler_pA;
	i64 *value_pA;

	sml_unit *unit_R1;
	i8 *scaler_R1;
	i64 *value_R1;

	sml_unit *unit_R4;
	i8 *scaler_R4;
	i64 *value_R4;

	octet_string *signature_pA_R1_R4;

	sml_unit *unit_mA;
	i8 *scaler_mA;
	i64 *value_mA;

	sml_unit *unit_R2;
	i8 *scaler_R2;
	i64 *value_R2;

	sml_unit *unit_R3;
	i8 *scaler_R3;
	i64 *value_R3;

	octet_string *signature_mA_R2_R3;
} sml_tupel_entry;

typedef struct {
	octet_string *obj_name;
	sml_unit *unit;
	i8 *scaler;
	sml_value *value;
	octet_string *value_signature;
} sml_period_entry;

typedef struct {
	u8 *tag;
	union {
		sml_value *value;
		sml_period_entry *period_entry;
		sml_tupel_entry *tupel_entry;
		sml_time *time;
	} data;
} sml_proc_par_value;

typedef struct s_tree {
	octet_string *parameter_name;
	sml_proc_par_value *parameter_value; // optional
	struct s_tree **child_list;          // optional

	int child_list_len;
} sml_tree;

typedef struct {
	int path_entries_len;
	octet_string **path_entries;
} sml_tree_path;

// sml_tree;
sml_tree *sml_tree_init();
sml_tree *sml_tree_parse(sml_buffer *buf);
void sml_tree_add_tree(sml_tree *base_tree, sml_tree *tree);
void sml_tree_write(sml_tree *tree, sml_buffer *buf);
void sml_tree_free(sml_tree *tree);

// sml_tree_path;
sml_tree_path *sml_tree_path_init();
sml_tree_path *sml_tree_path_parse(sml_buffer *buf);
void sml_tree_path_add_path_entry(sml_tree_path *tree_path, octet_string *entry);
void sml_tree_path_write(sml_tree_path *tree_path, sml_buffer *buf);
void sml_tree_path_free(sml_tree_path *tree_path);

// sml_proc_par_value;
sml_proc_par_value *sml_proc_par_value_init();
sml_proc_par_value *sml_proc_par_value_parse(sml_buffer *buf);
void sml_proc_par_value_write(sml_proc_par_value *value, sml_buffer *buf);
void sml_proc_par_value_free(sml_proc_par_value *value);

// sml_tupel_entry;
sml_tupel_entry *sml_tupel_entry_init();
sml_tupel_entry *sml_tupel_entry_parse(sml_buffer *buf);
void sml_tupel_entry_write(sml_tupel_entry *tupel, sml_buffer *buf);
void sml_tupel_entry_free(sml_tupel_entry *tupel);

// sml_period_entry;
sml_period_entry *sml_period_entry_init();
sml_period_entry *sml_period_entry_parse(sml_buffer *buf);
void sml_period_entry_write(sml_period_entry *period, sml_buffer *buf);
void sml_period_entry_free(sml_period_entry *period);

#ifdef __cplusplus
}
#endif

#endif /* SML_TREE_H_ */
