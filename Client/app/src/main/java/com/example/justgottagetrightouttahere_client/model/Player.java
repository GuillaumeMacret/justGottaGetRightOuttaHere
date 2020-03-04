package com.example.justgottagetrightouttahere_client.model;

public class Player {
    public int posX, posY, id, roleId;

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


    public Player(int posX, int posY, int id, int roleId){
        this.posX = posX;
        this.posY = posY;
        this.id = id;
        this.roleId = roleId;
        posX = posY = 0;
    }
}
