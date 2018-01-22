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


#include <sml/sml_tree.h>
#include <sml/sml_value.h>
#include <stdio.h>

// sml_tree_path;

sml_tree_path *sml_tree_path_init() {
	sml_tree_path *tree_path = (sml_tree_path *) malloc(sizeof(sml_tree_path));
	*tree_path = ( sml_tree_path ) {
		.path_entries_len = 0,
		.path_entries = NULL
	};

	return tree_path;
}

sml_tree_path *sml_tree_path_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return 0;
	}

	sml_tree_path *tree_path = sml_tree_path_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		return 0;
	}

	octet_string *s;
	int elems;
	for (elems = sml_buf_get_next_length(buf); elems > 0; elems--) {
		s = sml_octet_string_parse(buf);
		if (sml_buf_has_errors(buf)) goto error;
		if (s) {
			sml_tree_path_add_path_entry(tree_path, s);
		}
	}

	return tree_path;

error:
	buf->error = 1;
	sml_tree_path_free(tree_path);
	return 0;
}

void sml_tree_path_add_path_entry(sml_tree_path *tree_path, octet_string *entry) {
	tree_path->path_entries_len++;
	tree_path->path_entries = (octet_string **) realloc(tree_path->path_entries,
		sizeof(octet_string *) * tree_path->path_entries_len);

	tree_path->path_entries[tree_path->path_entries_len - 1] = entry;
}

void sml_tree_path_write(sml_tree_path *tree_path, sml_buffer *buf) {
	if (tree_path == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	if (tree_path->path_entries && tree_path->path_entries_len > 0) {
		sml_buf_set_type_and_length(buf, SML_TYPE_LIST, tree_path->path_entries_len);
		
		int i;
		for (i = 0; i < tree_path->path_entries_len; i++) {
			sml_octet_string_write(tree_path->path_entries[i], buf);
		}
	}
}

void sml_tree_path_free(sml_tree_path *tree_path) {
	if (tree_path) {
		if (tree_path->path_entries && tree_path->path_entries_len > 0) {
			int i;
			for (i = 0; i < tree_path->path_entries_len; i++) {
				sml_octet_string_free(tree_path->path_entries[i]);
			}

			free(tree_path->path_entries);
		}

		free(tree_path);
	}
}


// sml_tree;

sml_tree *sml_tree_init() {
	sml_tree *tree = (sml_tree *) malloc(sizeof(sml_tree));
	*tree = ( sml_tree ) {
		.parameter_name = NULL,
		.parameter_value = NULL,
		.child_list = NULL,
		.child_list_len = 0
	};

	return tree;
}

sml_tree *sml_tree_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return 0;
	}

	sml_tree *tree = sml_tree_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 3) {
		buf->error = 1;
		goto error;
	}

	tree->parameter_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tree->parameter_value = sml_proc_par_value_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	if (!sml_buf_optional_is_skipped(buf)) {
		if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
			buf->error = 1;
			goto error;
		}

		sml_tree *c;
		int elems;
		for (elems = sml_buf_get_next_length(buf); elems > 0; elems--) {
			c = sml_tree_parse(buf);
			if (sml_buf_has_errors(buf)) goto error;
			if (c) {
				sml_tree_add_tree(tree, c);
			}
		}
	}

	return tree;

error:
	sml_tree_free(tree);
	return 0;
}

void sml_tree_add_tree(sml_tree *base_tree, sml_tree *tree) {
	base_tree->child_list_len++;
	base_tree->child_list = (sml_tree **) realloc(base_tree->child_list,
		sizeof(sml_tree *) * base_tree->child_list_len);
	base_tree->child_list[base_tree->child_list_len - 1] = tree;
}

void sml_tree_free(sml_tree *tree) {
	if (tree) {
		sml_octet_string_free(tree->parameter_name);
		sml_proc_par_value_free(tree->parameter_value);
		int i;
		for (i = 0; i < tree->child_list_len; i++) {
			sml_tree_free(tree->child_list[i]);
		}

		free(tree->child_list);
		free(tree);
	}
}

void sml_tree_write(sml_tree *tree, sml_buffer *buf) {
	if (tree == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 3);

	sml_octet_string_write(tree->parameter_name, buf);
	sml_proc_par_value_write(tree->parameter_value, buf);

	if (tree->child_list && tree->child_list_len > 0) {
		sml_buf_set_type_and_length(buf, SML_TYPE_LIST, tree->child_list_len);

		int i;
		for (i = 0; i < tree->child_list_len; i++) {
			sml_tree_write(tree->child_list[i], buf);
		}
	}
	else {
		sml_buf_optional_write(buf);
	}
}


// sml_proc_par_value;

sml_proc_par_value *sml_proc_par_value_init() {
	sml_proc_par_value *value = (sml_proc_par_value *) malloc(sizeof(sml_proc_par_value));
	*value = ( sml_proc_par_value ) {
		.tag = NULL,
		.data.value = NULL
	};
	return value;
}

sml_proc_par_value *sml_proc_par_value_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return 0;
	}

	sml_proc_par_value *ppv = sml_proc_par_value_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 2) {
		buf->error = 1;
		goto error;
	}

	ppv->tag = sml_u8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	switch (*(ppv->tag)) {
		case SML_PROC_PAR_VALUE_TAG_VALUE:
			ppv->data.value = sml_value_parse(buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_PERIOD_ENTRY:
			ppv->data.period_entry = sml_period_entry_parse(buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_TUPEL_ENTRY:
			ppv->data.tupel_entry = sml_tupel_entry_parse(buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_TIME:
			ppv->data.time = sml_time_parse(buf);
			break;
		default:
			buf->error = 1;
			goto error;
	}

	return ppv;

error:
	sml_proc_par_value_free(ppv);
	return 0;
}

void sml_proc_par_value_write(sml_proc_par_value *value, sml_buffer *buf) {
	if (value == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 2);
	sml_u8_write(value->tag, buf);

	switch (*(value->tag)) {
		case SML_PROC_PAR_VALUE_TAG_VALUE:
			sml_value_write(value->data.value, buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_PERIOD_ENTRY:
			sml_period_entry_write(value->data.period_entry, buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_TUPEL_ENTRY:
			sml_tupel_entry_write(value->data.tupel_entry, buf);
			break;
		case SML_PROC_PAR_VALUE_TAG_TIME:
			sml_time_write(value->data.time, buf);
			break;
		default:
			fprintf(stderr,"libsml: error: unknown tag in %s\n", __FUNCTION__);
	}
}

void sml_proc_par_value_free(sml_proc_par_value *ppv) {
	if (ppv) {
		if (ppv->tag) {
			switch (*(ppv->tag)) {
				case SML_PROC_PAR_VALUE_TAG_VALUE:
					sml_value_free(ppv->data.value);
					break;
				case SML_PROC_PAR_VALUE_TAG_PERIOD_ENTRY:
					sml_period_entry_free(ppv->data.period_entry);
					break;
				case SML_PROC_PAR_VALUE_TAG_TUPEL_ENTRY:
					sml_tupel_entry_free(ppv->data.tupel_entry);
					break;
				case SML_PROC_PAR_VALUE_TAG_TIME:
					sml_time_free(ppv->data.time);
					break;
				default:
					if (ppv->data.value) {
						free(ppv->data.value);
					}
			}
			sml_number_free(ppv->tag);
		}
		else {
			// Without the tag, there might be a memory leak.
			if (ppv->data.value) {
				free(ppv->data.value);
			}
		}

		free(ppv);
	}
}


// sml_tuple_entry;

sml_tupel_entry *sml_tupel_entry_init() {
	sml_tupel_entry *tupel = (sml_tupel_entry *) malloc(sizeof(sml_tupel_entry));
	*tupel = ( sml_tupel_entry ) {
		.server_id = NULL,
		.sec_index = NULL,
		.status = NULL,
		.unit_pA = NULL,
		.scaler_pA = NULL,
		.value_pA = NULL,
		.unit_R1 = NULL,
		.scaler_R1 = NULL,
		.value_R1 = NULL,
		.unit_R4 = NULL,
		.scaler_R4 = NULL,
		.value_R4 = NULL,
		.signature_pA_R1_R4 = NULL,
		.unit_mA = NULL,
		.scaler_mA = NULL,
		.value_mA = NULL,
		.unit_R2 = NULL,
		.scaler_R2 = NULL,
		.value_R2 = NULL,
		.unit_R3 = NULL,
		.scaler_R3 = NULL,
		.value_R3 = NULL,
		.signature_mA_R2_R3 = NULL
	};

	return tupel;
}

sml_tupel_entry *sml_tupel_entry_parse(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return 0;
	}

	sml_tupel_entry *tupel = sml_tupel_entry_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 23) {
		buf->error = 1;
		goto error;
	}

	tupel->server_id = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->sec_index = sml_time_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->status = sml_u64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_pA = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_pA = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_pA = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_R1 = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_R1 = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_R1 = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_R4 = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_R4 = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_R4 = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->signature_pA_R1_R4 = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_mA = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_mA = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_mA = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_R2 = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_R2 = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_R2 = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->unit_R3 = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->scaler_R3 = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;
	tupel->value_R3 = sml_i64_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	tupel->signature_mA_R2_R3 = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	return tupel;

error:
	sml_tupel_entry_free(tupel);
	return 0;
}

void sml_tupel_entry_write(sml_tupel_entry *tupel, sml_buffer *buf) {
	if (tupel == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 23);

	sml_octet_string_write(tupel->server_id, buf);
	sml_time_write(tupel->sec_index, buf);
	sml_u64_write(tupel->status, buf);

	sml_unit_write(tupel->unit_pA, buf);
	sml_i8_write(tupel->scaler_pA, buf);
	sml_i64_write(tupel->value_pA, buf);

	sml_unit_write(tupel->unit_R1, buf);
	sml_i8_write(tupel->scaler_R1, buf);
	sml_i64_write(tupel->value_R1, buf);

	sml_unit_write(tupel->unit_R4, buf);
	sml_i8_write(tupel->scaler_R4, buf);
	sml_i64_write(tupel->value_R4, buf);

	sml_octet_string_write(tupel->signature_pA_R1_R4, buf);

	sml_unit_write(tupel->unit_mA, buf);
	sml_i8_write(tupel->scaler_mA, buf);
	sml_i64_write(tupel->value_mA, buf);

	sml_unit_write(tupel->unit_R2, buf);
	sml_i8_write(tupel->scaler_R2, buf);
	sml_i64_write(tupel->value_R2, buf);

	sml_unit_write(tupel->unit_R3, buf);
	sml_i8_write(tupel->scaler_R3, buf);
	sml_i64_write(tupel->value_R3, buf);

	sml_octet_string_write(tupel->signature_mA_R2_R3, buf);
}

void sml_tupel_entry_free(sml_tupel_entry *tupel) {
	if (tupel) {
		sml_octet_string_free(tupel->server_id);
		sml_time_free(tupel->sec_index);
		sml_number_free(tupel->status);

		sml_unit_free(tupel->unit_pA);
		sml_number_free(tupel->scaler_pA);
		sml_number_free(tupel->value_pA);

		sml_unit_free(tupel->unit_R1);
		sml_number_free(tupel->scaler_R1);
		sml_number_free(tupel->value_R1);

		sml_unit_free(tupel->unit_R4);
		sml_number_free(tupel->scaler_R4);
		sml_number_free(tupel->value_R4);

		sml_octet_string_free(tupel->signature_pA_R1_R4);

		sml_unit_free(tupel->unit_mA);
		sml_number_free(tupel->scaler_mA);
		sml_number_free(tupel->value_mA);

		sml_unit_free(tupel->unit_R2);
		sml_number_free(tupel->scaler_R2);
		sml_number_free(tupel->value_R2);

		sml_unit_free(tupel->unit_R3);
		sml_number_free(tupel->scaler_R3);
		sml_number_free(tupel->value_R3);

		sml_octet_string_free(tupel->signature_mA_R2_R3);

		free(tupel);
	}
}



// sml_period_entry;

sml_period_entry *sml_period_entry_init() {
	sml_period_entry *period = (sml_period_entry *) malloc(sizeof(sml_period_entry));
	*period = ( sml_period_entry ) {
		.obj_name = NULL,
		.unit = NULL,
		.scaler = NULL,
		.value = NULL,
		.value_signature = NULL
	};

	return period;
}

static void * sml_period_entry_parse_(sml_buffer *buf) {
	if (sml_buf_optional_is_skipped(buf)) {
		return 0;
	}

	sml_period_entry *period = sml_period_entry_init();

	if (sml_buf_get_next_type(buf) != SML_TYPE_LIST) {
		buf->error = 1;
		goto error;
	}

	if (sml_buf_get_next_length(buf) != 5) {
		buf->error = 1;
		goto error;
	}

	period->obj_name = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	period->unit = sml_unit_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	period->scaler = sml_i8_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	period->value = sml_value_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	period->value_signature = sml_octet_string_parse(buf);
	if (sml_buf_has_errors(buf)) goto error;

	return period;

error:
	sml_period_entry_free(period);
	return 0;
}

sml_period_entry * sml_period_entry_parse( sml_buffer * buf ) {
	return sml_period_entry_parse_( buf );
}

void sml_period_entry_write(sml_period_entry *period, sml_buffer *buf) {
	if (period == 0) {
		sml_buf_optional_write(buf);
		return;
	}

	sml_buf_set_type_and_length(buf, SML_TYPE_LIST, 5);

	sml_octet_string_write(period->obj_name, buf);
	sml_unit_write(period->unit, buf);
	sml_i8_write(period->scaler, buf);
	sml_value_write(period->value, buf);
	sml_octet_string_write(period->value_signature, buf);
}

static void sml_period_entry_free_( void * p ) {
	sml_period_entry * period = p;

	if (period) {
		sml_octet_string_free(period->obj_name);
		sml_unit_free(period->unit);
		sml_number_free(period->scaler);
		sml_value_free(period->value);
		sml_octet_string_free(period->value_signature);

		free(period);
	}
}

void sml_period_entry_free( sml_period_entry * period ) {
	sml_period_entry_free_( period );
}
