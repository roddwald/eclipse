#pragma once

#ifndef bspflags_h
#define bspflags_h

#ifdef _win32
#pragma once
#endif

#define	contents_empty			0

#define	contents_solid			0x1
#define	contents_window			0x2
#define	contents_aux			0x4
#define	contents_grate			0x8
#define	contents_slime			0x10
#define	contents_water			0x20
#define	contents_blocklos		0x40
#define contents_opaque			0x80
#define	last_visible_contents	0x80

#define all_visible_contents (last_visible_contents | (last_visible_contents-1))

#define contents_testfogvolume	0x100
#define contents_unused			0x200

#define contents_unused6		0x400

#define contents_team1			0x800
#define contents_team2			0x1000

#define contents_ignore_nodraw_opaque	0x2000

#define contents_moveable		0x4000

#define	contents_areaportal		0x8000

#define	contents_playerclip		0x10000
#define	contents_monsterclip	0x20000

#define	contents_current_0		0x40000
#define	contents_current_90		0x80000
#define	contents_current_180	0x100000
#define	contents_current_270	0x200000
#define	contents_current_up		0x400000
#define	contents_current_down	0x800000

#define	contents_origin			0x1000000

#define	contents_monster		0x2000000
#define	contents_debris			0x4000000
#define	contents_detail			0x8000000
#define	contents_translucent	0x10000000
#define	contents_ladder			0x20000000
#define contents_hitbox			0x40000000

#define	surf_light		0x0001
#define	surf_sky2d		0x0002
#define	surf_sky		0x0004
#define	surf_warp		0x0008
#define	surf_trans		0x0010
#define surf_noportal	0x0020
#define	surf_trigger	0x0040
#define	surf_nodraw		0x0080

#define	surf_hint		0x0100

#define	surf_skip		0x0200
#define surf_nolight	0x0400
#define surf_bumplight	0x0800
#define surf_noshadows	0x1000
#define surf_nodecals	0x2000
#define surf_nochop		0x4000
#define surf_hitbox		0x8000

#define	mask_all					(0xFFFFFFFF)
#define	mask_solid					(contents_solid|contents_moveable|contents_window|contents_monster|contents_grate)
#define	mask_playersolid			(contents_solid|contents_moveable|contents_playerclip|contents_window|contents_monster|contents_grate)
#define	mask_npcsolid				(contents_solid|contents_moveable|contents_monsterclip|contents_window|contents_monster|contents_grate)
#define	mask_water					(contents_water|contents_moveable|contents_slime)
#define	mask_opaque					(contents_solid|contents_moveable|contents_opaque)
#define mask_opaque_and_npcs		(mask_opaque|contents_monster)
#define mask_blocklos				(contents_solid|contents_moveable|contents_blocklos)
#define mask_blocklos_and_npcs		(mask_blocklos|contents_monster)
#define	mask_visible					(mask_opaque|contents_ignore_nodraw_opaque)
#define mask_visible_and_npcs		(mask_opaque_and_npcs|contents_ignore_nodraw_opaque)
#define	mask_shot					(contents_solid|contents_moveable|contents_monster|contents_window|contents_debris|contents_hitbox)
#define mask_shot_hull				(contents_solid|contents_moveable|contents_monster|contents_window|contents_debris|contents_grate)
#define mask_shot_portal			(contents_solid|contents_moveable|contents_window|contents_monster)
#define mask_solid_brushonly		(contents_solid|contents_moveable|contents_window|contents_grate)
#define mask_playersolid_brushonly	(contents_solid|contents_moveable|contents_window|contents_playerclip|contents_grate)
#define mask_npcsolid_brushonly		(contents_solid|contents_moveable|contents_window|contents_monsterclip|contents_grate)
#define mask_npcworldstatic			(contents_solid|contents_window|contents_monsterclip|contents_grate)
#define mask_splitareaportal		(contents_water|contents_slime)

#define mask_current				(contents_current_0|contents_current_90|contents_current_180|contents_current_270|contents_current_up|contents_current_down)

#define	mask_deadsolid				(contents_solid|contents_playerclip|contents_window|contents_grate)

#endif  