#pragma once

struct c_globals {
	float m_realtime; // 0
	int m_framecount; // 4
	float m_abs_frametime; // 8
	float m_abs_framestarttimestddev; // 12
	float m_curtime; // 16
	float m_frametime; // 20
	int m_max_clients; // 24
	int m_tickcount; // 28
	float m_ipt; // 32
	float m_interp;
};