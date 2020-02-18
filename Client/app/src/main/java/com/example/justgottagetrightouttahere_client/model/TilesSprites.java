package com.example.justgottagetrightouttahere_client.model;

import android.graphics.drawable.Drawable;
import android.media.Image;

import com.example.justgottagetrightouttahere_client.R;

import java.util.HashMap;
import java.util.Map;

public class TilesSprites {
    public static Map<Integer, Integer> tilesSpritesMap;


    static boolean loaded = false;
    /**
     * Loads the sprites in memory if not already done
     */
    public static void LoadSprites(){
        if(loaded) return;
        tilesSpritesMap = new HashMap<>();
        tilesSpritesMap.put(0, R.drawable.floor_0);
        tilesSpritesMap.put(1, R.drawable.floor_1);

        loaded = true;
    }
}
