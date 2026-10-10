################################################################
#
# $Id:$
#
# $Log:$
#
CC=  gcc  # gcc or g++

CFLAGS=-std=c17 -g -Wall -DNORMALUNIX
LDFLAGS=
LIBS=-luser32 -lgdi32 -lws2_32 -lm

# subdirectory for objects
O=win32



# not too sophisticated dependency
OBJS=				\
		$(O)/doomdef.o		\
		$(O)/doomstat.o		\
		$(O)/dstrings.o		\
		$(O)/i_system.o		\
		$(O)/i_video.o		\
		$(O)/i_net.o		\
		$(O)/i_sound.o		\
		$(O)/tables.o		\
		$(O)/f_finale.o		\
		$(O)/f_wipe.o 		\
		$(O)/d_main.o		\
		$(O)/d_net.o		\
		$(O)/d_items.o		\
		$(O)/g_game.o		\
		$(O)/m_menu.o		\
		$(O)/m_misc.o		\
		$(O)/m_argv.o  		\
		$(O)/m_bbox.o		\
		$(O)/m_fixed.o		\
		$(O)/m_swap.o		\
		$(O)/m_cheat.o		\
		$(O)/m_random.o		\
		$(O)/am_map.o		\
		$(O)/p_ceilng.o		\
		$(O)/p_doors.o		\
		$(O)/p_enemy.o		\
		$(O)/p_floor.o		\
		$(O)/p_inter.o		\
		$(O)/p_lights.o		\
		$(O)/p_map.o		\
		$(O)/p_maputl.o		\
		$(O)/p_plats.o		\
		$(O)/p_pspr.o		\
		$(O)/p_setup.o		\
		$(O)/p_sight.o		\
		$(O)/p_spec.o		\
		$(O)/p_switch.o		\
		$(O)/p_mobj.o		\
		$(O)/p_telept.o		\
		$(O)/p_tick.o		\
		$(O)/p_saveg.o		\
		$(O)/p_user.o		\
		$(O)/r_bsp.o		\
		$(O)/r_data.o		\
		$(O)/r_draw.o		\
		$(O)/r_main.o		\
		$(O)/r_plane.o		\
		$(O)/r_segs.o		\
		$(O)/r_sky.o		\
		$(O)/r_things.o		\
		$(O)/w_wad.o		\
		$(O)/wi_stuff.o		\
		$(O)/v_video.o		\
		$(O)/st_lib.o		\
		$(O)/st_stuff.o		\
		$(O)/hu_stuff.o		\
		$(O)/hu_lib.o		\
		$(O)/s_sound.o		\
		$(O)/z_zone.o		\
		$(O)/info.o			\
		$(O)/sounds.o

all: $(O)/doom32.exe

$(O):
	mkdir $(O)

clean:
	if exist $(O)\* del /Q $(O)\*

$(O)/doom32.exe: $(OBJS) $(O)/i_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(O)/i_main.o \
	-o $(O)/doom32.exe $(LIBS)

$(O)/%.o: %.c | $(O)
	$(CC) $(CFLAGS) -c $< -o $@

#############################################################
#
#############################################################
