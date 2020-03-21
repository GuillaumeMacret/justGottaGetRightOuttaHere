package com.example.justgottagetrightouttahere_client.model;

import com.example.justgottagetrightouttahere_client.R;

import java.util.HashMap;
import java.util.Map;

public class ResourcesMaps {
    public static Map<Integer, Integer> blocksSpritesMap;
    public static Map<Integer, Integer> objectsSpritesMap;
    public static Map<Integer, Integer> playerSpritesMap;

    static boolean loaded = false;
    /**
     * Loads the sprites in memory if not already done
     */
    public static void LoadSprites(){
        if(loaded) return;
        blocksSpritesMap = new HashMap<>();
        objectsSpritesMap = new HashMap<>();
        playerSpritesMap = new HashMap<>();

        blocksSpritesMap.put(0, R.drawable.floor_0);
        blocksSpritesMap.put(1, R.drawable.floor_1);

        objectsSpritesMap.put(1,R.drawable.object_1);

        playerSpritesMap.put(0,R.drawable.player0_idle);
        playerSpritesMap.put(1,R.drawable.player1_idle);
        playerSpritesMap.put(2,R.drawable.player2_idle);
        playerSpritesMap.put(3,R.drawable.player3_idle);

        loaded = true;
    }
}
