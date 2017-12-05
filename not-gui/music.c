/*
 * music.c
 *
 *  Created on: Dec 4, 2017
 *      Author: anngould
 */

#include "open_interface.h"
#include "music.h"

void load_songs(){

//    oi_t* sensor = oi_alloc();
//    oi_init(sensor);
    unsigned char numNotes = 13;
    unsigned char notes[13] = {74,77,86,74,77,86,88,89,88,89,88,85,81};
    unsigned char duration[13] = {12,12,18,12,12,18,10,8,8,8,8,8,15};
    oi_loadSong(SONG_OF_STORMS, numNotes, notes, duration);
   // oi_play_song(SONG_OF_STORMS);
}


