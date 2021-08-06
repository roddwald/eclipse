#pragma once
#include "../utils/vfunc.hpp"

struct recv_table_t;
struct recv_prop_t;

struct recv_table_t {
	recv_prop_t* m_props;
	int m_num_props;
	void* m_decoder;
	char* m_net_table_name;
	PAD( 2 );
};

struct recv_prop_t {
	char* m_var_name;
	int m_recv_type;
	int m_flags;
	int m_string_buffer_size;
	bool m_inside_array;
	const void* m_extra_data;
	recv_prop_t* m_array_prop;
	void* m_array_length_proxy;
	void* m_proxy_fn;
	void* m_data_table_proxy_fn;
	recv_table_t* m_data_table;
	int m_offset;
	int m_element_stride;
	int m_num_elements;
	const char* m_parent_array_prop_name;
};

class recv_proxy_data_t {
public:
	const recv_prop_t* m_prop;
	char m_value;
	int	m_element;
	int m_object_id;
};

using create_client_class_t = void* ( * )( int ent_num, int serial_num );
using create_event_t = void* ( * )( );

class client_class_t {
public:
	create_client_class_t m_create_fn;
	create_event_t m_create_event_fn;
	char* m_network_name;
	recv_table_t* m_recv_table;
	client_class_t* m_next;
	int m_class_id;
};

class c_client {
public:
	client_class_t* get_all_classes( ) {
		using get_all_classes_fn = client_class_t * ( __thiscall* )( void* );
		return vfunc< get_all_classes_fn >( this, 8 )( this );
	}
};