package com.example.justgottagetrightouttahere_client.model;

public class Player {
    int posX, posY, id, roleId;

    Player(){
        posX = posY = id = roleId = 0;
    }

    /**
     * @param id
     * @param roleId
     */
    Player(int id, int roleId){
        this.id = id;
        this.roleId = roleId;
        posX = posY = 0;
    }
}
