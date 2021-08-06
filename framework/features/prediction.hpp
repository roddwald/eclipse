#pragma once
#include "../sdk/sdk.hpp"

namespace prediction_util {
	inline float frametime;
	inline float curtime;
	inline int tickcount;
	inline int flags;
	inline vec3_t velocity;
	inline void* movedata;
	inline uintptr_t prediction_player;
	inline uintptr_t prediction_seed;
	inline bool first_time_pred;
	inline bool in_pred;
}

namespace features {
	namespace prediction {
		namespace backup {
			inline float packet_end_cmdnum = 0.f;
			inline float velocity_modifier = 0.f;
		}

		struct netvars_t {
			vec3_t m_aim_punch;
			vec3_t m_aim_punch_vel;
			vec3_t m_view_punch;
			vec3_t m_view_offset;
			vec3_t m_base_velocity;
			vec3_t m_velocity;
			vec3_t m_origin;
			float m_fall_vel;
			float m_duck_amount;
			float m_duck_speed;
			float m_vel_modifier;
			uint32_t m_ground_ent;
			movetypes m_move_type;
			int m_flags;

			float m_accuracy_penalty;
			float m_recoil_index;
			bool m_weapon_valid;

			int m_tick_base;

			bool store(player_t* ent) {
				bool updated = false;

				if (!ent || !ent->alive())
					return updated;

				const auto weapon = ent->weapon();

				if (m_aim_punch != ent->aim_punch()) updated = true;
				if (m_aim_punch_vel != ent->aim_punch_vel()) updated = true;
				if (m_view_punch != ent->view_punch()) updated = true;
				if (m_view_offset != ent->view_offset()) updated = true;
				if (m_base_velocity != ent->base_vel()) updated = true;
				if (m_velocity != ent->vel()) updated = true;
				if (m_origin != ent->origin()) updated = true;
				if (m_fall_vel != ent->fall_vel()) updated = true;
				if (m_duck_amount != ent->crouch_amount()) updated = true;
				if (m_duck_speed != ent->crouch_speed()) updated = true;
				if (m_vel_modifier != ent->velocity_modifier()) updated = true;
				if (m_ground_ent != ent->ground_entity_handle()) updated = true;
				if (m_move_type != ent->movetype()) updated = true;
				if (m_flags != ent->flags()) updated = true;

				if (weapon && m_weapon_valid) {
					if (m_accuracy_penalty != weapon->accuracy_penalty()) updated = true;
					if (m_recoil_index != weapon->recoil_index()) updated = true;
				}

				m_aim_punch = ent->aim_punch();
				m_aim_punch_vel = ent->aim_punch_vel();
				m_view_punch = ent->view_punch();
				m_view_offset = ent->view_offset();
				m_base_velocity = ent->base_vel();
				m_velocity = ent->vel();
				m_origin = ent->origin();
				m_fall_vel = ent->fall_vel();
				m_duck_amount = ent->crouch_amount();
				m_duck_speed = ent->crouch_speed();
				m_vel_modifier = ent->velocity_modifier();
				m_ground_ent = ent->ground_entity_handle();
				m_move_type = ent->movetype();
				m_flags = ent->flags();

				if (weapon) {
					m_accuracy_penalty = weapon->accuracy_penalty();
					m_recoil_index = weapon->recoil_index();
					m_weapon_valid = true;
				}
				else {
					m_weapon_valid = false;
				}

				m_tick_base = ent->tick_base();
				return updated;
			}

			bool restore(player_t* ent) {
				bool updated = false;

				if (!ent || !ent->alive())
					return updated;

				const auto weapon = ent->weapon();

				if (abs(ent->aim_punch().x - m_aim_punch.x) < 0.03125f && abs(ent->aim_punch().y - m_aim_punch.y) < 0.03125f && abs(ent->aim_punch().z - m_aim_punch.z) < 0.03125f)
					ent->aim_punch() = m_aim_punch, updated = true;

				if (abs(ent->aim_punch_vel().x - m_aim_punch_vel.x) < 0.03125f && abs(ent->aim_punch_vel().y - m_aim_punch_vel.y) < 0.03125f && abs(ent->aim_punch_vel().z - m_aim_punch_vel.z) < 0.03125f)
					ent->aim_punch_vel() = m_aim_punch_vel, updated = true;

				if (abs(ent->view_punch().x - m_view_punch.x) < 0.03125f && abs(ent->view_punch().y - m_view_punch.y) < 0.03125f && abs(ent->view_punch().z - m_view_punch.z) < 0.03125f)
					ent->view_punch() = m_view_punch, updated = true;

				if (abs(ent->view_offset().x - m_view_offset.x) < 0.03125f && abs(ent->view_offset().y - m_view_offset.y) < 0.03125f && abs(ent->view_offset().z - m_view_offset.z) < 0.03125f)
					ent->view_offset() = m_view_offset, updated = true;

				if (abs(ent->base_vel().x - m_base_velocity.x) < 0.03125f && abs(ent->base_vel().y - m_base_velocity.y) < 0.03125f && abs(ent->base_vel().z - m_base_velocity.z) < 0.03125f)
					ent->base_vel() = m_base_velocity, updated = true;

				if (abs(ent->vel().x - m_velocity.x) < 0.03125f && abs(ent->vel().y - m_velocity.y) < 0.03125f && abs(ent->vel().z - m_velocity.z) < 0.03125f)
					ent->vel() = m_velocity, updated = true;

				if (abs(ent->origin().x - m_origin.x) < 0.03125f && abs(ent->origin().y - m_origin.y) < 0.03125f && abs(ent->origin().z - m_origin.z) < 0.03125f)
					ent->origin() = m_origin, updated = true;

				if (abs(ent->fall_vel() - m_fall_vel) < 0.03125f)
					ent->fall_vel() = m_fall_vel, updated = true;

				if (abs(ent->crouch_amount() - m_duck_amount) < 0.03125f)
					ent->crouch_amount() = m_duck_amount, updated = true;

				if (abs(ent->crouch_speed() - m_duck_speed) < 0.03125f)
					ent->crouch_speed() = m_duck_speed, updated = true;

				if (abs(ent->velocity_modifier() - m_vel_modifier) < 0.03125f)
					ent->velocity_modifier() = m_vel_modifier, updated = true;

				if (ent->ground_entity_handle() != m_ground_ent)
					ent->ground_entity_handle() = m_ground_ent, updated = true;

				if (ent->movetype() != m_move_type)
					ent->movetype() = m_move_type, updated = true;

				if (ent->flags() != m_flags)
					ent->flags() = m_flags, updated = true;

				if (weapon && m_weapon_valid) {
					if (abs(weapon->accuracy_penalty() - m_accuracy_penalty) < 0.03125f)
						weapon->accuracy_penalty() = m_accuracy_penalty, updated = true;

					if (abs(weapon->recoil_index() - m_recoil_index) < 0.03125f)
						weapon->recoil_index() = m_recoil_index, updated = true;
				}

				return updated;
			}

			void reapply(player_t* ent) {
				if (!ent || !ent->alive())
					return;

				const auto weapon = ent->weapon();

				ent->aim_punch() = m_aim_punch;
				ent->aim_punch_vel() = m_aim_punch_vel;
				ent->view_punch() = m_view_punch;
				ent->view_offset() = m_view_offset;
				ent->base_vel() = m_base_velocity;
				ent->vel() = m_velocity;
				ent->origin() = m_origin;
				ent->fall_vel() = m_fall_vel;
				ent->crouch_amount() = m_duck_amount;
				ent->crouch_speed() = m_duck_speed;
				ent->velocity_modifier() = m_vel_modifier;
				ent->ground_entity_handle() = m_ground_ent;
				ent->movetype() = m_move_type;
				ent->flags() = m_flags;

				if (weapon && m_weapon_valid) {
					weapon->accuracy_penalty() = m_accuracy_penalty;
					weapon->recoil_index() = m_recoil_index;
				}
			}
		};

		inline bool in_prediction = false;
		inline float vel_modifier = 0.0f;
		inline float crouch_amount = 0.0f;
		inline vec3_t vel = vec3_t(0.0f, 0.0f, 0.0f);

		float curtime();

		int shift_tickbase();
		void update(int stage);
		void fix_viewmodel(bool store = false);
		bool fix_netvars(int cmd, bool store = false);
		void run(const std::function< void() >& fn);
		float calculate_velocity_modifier(player_t* local, int m_cmd_num, bool pre_orig);
	}
}