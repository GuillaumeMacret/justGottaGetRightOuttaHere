package com.example.justgottagetrightouttahere_client.Constants;

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

        blocksSpritesMap.put(0, R.drawable.tile_0);
        blocksSpritesMap.put(1, R.drawable.tile_1);
        blocksSpritesMap.put(2, R.drawable.tile_2);
        blocksSpritesMap.put(3, R.drawable.tile_3);
        blocksSpritesMap.put(4, R.drawable.tile_4);
        blocksSpritesMap.put(5, R.drawable.tile_5);
        blocksSpritesMap.put(6, R.drawable.tile_6);
        blocksSpritesMap.put(7, R.drawable.tile_7);
        blocksSpritesMap.put(8, R.drawable.tile_8);
        blocksSpritesMap.put(9, R.drawable.tile_9);

        blocksSpritesMap.put(10, R.drawable.tile_10);
        blocksSpritesMap.put(11, R.drawable.tile_11);
        blocksSpritesMap.put(12, R.drawable.tile_12);
        blocksSpritesMap.put(13, R.drawable.tile_13);
        blocksSpritesMap.put(14, R.drawable.tile_14);
        blocksSpritesMap.put(15, R.drawable.tile_15);
        blocksSpritesMap.put(16, R.drawable.tile_16);
        blocksSpritesMap.put(17, R.drawable.tile_17);
        blocksSpritesMap.put(18, R.drawable.tile_18);
        blocksSpritesMap.put(19, R.drawable.tile_19);

        blocksSpritesMap.put(20, R.drawable.tile_20);
        blocksSpritesMap.put(21, R.drawable.tile_21);
        blocksSpritesMap.put(22, R.drawable.tile_22);
        blocksSpritesMap.put(23, R.drawable.tile_23);
        blocksSpritesMap.put(24, R.drawable.tile_24);
        blocksSpritesMap.put(25, R.drawable.tile_25);
        blocksSpritesMap.put(26, R.drawable.tile_26);
        blocksSpritesMap.put(27, R.drawable.tile_27);
        blocksSpritesMap.put(28, R.drawable.tile_28);
        blocksSpritesMap.put(29, R.drawable.tile_29);

        blocksSpritesMap.put(30, R.drawable.tile_30);
        blocksSpritesMap.put(31, R.drawable.tile_31);
        blocksSpritesMap.put(32, R.drawable.tile_32);
        blocksSpritesMap.put(33, R.drawable.tile_33);
        blocksSpritesMap.put(34, R.drawable.tile_34);
        blocksSpritesMap.put(35, R.drawable.tile_35);
        blocksSpritesMap.put(36, R.drawable.tile_36);
        blocksSpritesMap.put(37, R.drawable.tile_37);
        blocksSpritesMap.put(38, R.drawable.tile_38);
        blocksSpritesMap.put(39, R.drawable.tile_39);

        blocksSpritesMap.put(40, R.drawable.tile_40);
        blocksSpritesMap.put(41, R.drawable.tile_41);
        blocksSpritesMap.put(42, R.drawable.tile_42);
        blocksSpritesMap.put(43, R.drawable.tile_43);
        blocksSpritesMap.put(44, R.drawable.tile_44);
        blocksSpritesMap.put(45, R.drawable.tile_45);
        blocksSpritesMap.put(46, R.drawable.tile_46);
        blocksSpritesMap.put(47, R.drawable.tile_47);
        blocksSpritesMap.put(48, R.drawable.tile_48);
        blocksSpritesMap.put(49, R.drawable.tile_49);

        blocksSpritesMap.put(50, R.drawable.tile_50);
        blocksSpritesMap.put(51, R.drawable.tile_51);
        blocksSpritesMap.put(52, R.drawable.tile_52);
        blocksSpritesMap.put(53, R.drawable.tile_53);
        blocksSpritesMap.put(54, R.drawable.tile_54);
        blocksSpritesMap.put(55, R.drawable.tile_55);
        blocksSpritesMap.put(56, R.drawable.tile_56);
        blocksSpritesMap.put(57, R.drawable.tile_57);
        blocksSpritesMap.put(58, R.drawable.tile_58);
        blocksSpritesMap.put(59, R.drawable.tile_59);

        blocksSpritesMap.put(60, R.drawable.tile_60);
        blocksSpritesMap.put(61, R.drawable.tile_61);
        blocksSpritesMap.put(62, R.drawable.tile_62);
        blocksSpritesMap.put(63, R.drawable.tile_63);
        blocksSpritesMap.put(64, R.drawable.tile_64);
        blocksSpritesMap.put(65, R.drawable.tile_65);
        blocksSpritesMap.put(66, R.drawable.tile_66);
        blocksSpritesMap.put(67, R.drawable.tile_67);
        blocksSpritesMap.put(68, R.drawable.tile_68);
        blocksSpritesMap.put(69, R.drawable.tile_69);

        blocksSpritesMap.put(70, R.drawable.tile_70);
        blocksSpritesMap.put(71, R.drawable.tile_71);
        /*
        blocksSpritesMap.put(X0, R.drawable.tile_X0);
        blocksSpritesMap.put(X1, R.drawable.tile_X1);
        blocksSpritesMap.put(X2, R.drawable.tile_X2);
        blocksSpritesMap.put(X3, R.drawable.tile_X3);
        blocksSpritesMap.put(X4, R.drawable.tile_X4);
        blocksSpritesMap.put(X5, R.drawable.tile_X5);
        blocksSpritesMap.put(X6, R.drawable.tile_X6);
        blocksSpritesMap.put(X7, R.drawable.tile_X7);
        blocksSpritesMap.put(X8, R.drawable.tile_X8);
        blocksSpritesMap.put(X9, R.drawable.tile_X9);

         */

        /*
        objectsSpritesMap.put(1,R.drawable.object_1);
         */

        playerSpritesMap.put(0,R.drawable.player0_idle);
        playerSpritesMap.put(1,R.drawable.player1_idle);
        playerSpritesMap.put(2,R.drawable.player2_idle);
        playerSpritesMap.put(3,R.drawable.player3_idle);

        loaded = true;
    }
}
