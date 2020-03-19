package com.example.justgottagetrightouttahere_client.model;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

/**
 * Class representing the game board and its players
 * This is NOT including the lobby
 */
public class GameModel {
    public int sizeX, sizeY;
    public int [][] gameMatrix;
    public List<Player> players;

    static Semaphore redrawMutex = new Semaphore(1);
    private boolean needsRedraw = false;

    /**
     * Creates the game with its matrix according to size given in parameter
     * @param sizeX
     * @param sizeY
     */
    public GameModel(int sizeX, int sizeY){
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        gameMatrix = new int[sizeX][sizeY];
        players = new ArrayList<>();
    }

    public GameModel(int matrix[][]){
        loadLevel(matrix);
    }

    public void movePlayer(int playerId, int xPos, int yPos){
        Log.e("INFO","Game model : Moving player "+playerId);
        players.get(playerId).posX = xPos;
        players.get(playerId).posY = yPos;
    }

    public void updateTiles(List<Tile>tiles){
        for(Tile t : tiles){
            gameMatrix[t.posX][t.posY] = t.spriteId;
        }
        callForRedraw();
    }

    public void loadLevel(int matrix[][]){
        Log.e("INFO","Loading a new level");
        sizeX = matrix.length;
        sizeY = matrix[0].length;
        gameMatrix = new int[sizeX][sizeY];
        for(int i = 0; i < matrix.length; ++i){
            for(int j = 0; j < matrix[i].length; ++j){
                gameMatrix[i][j] = matrix[i][j];
            }
        }
        callForRedraw();
    }

    private void callForRedraw(){
        try {
            redrawMutex.acquire();
            needsRedraw = true;
            redrawMutex.release();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void setRedrawDone(){
        try {
            redrawMutex.acquire();
            needsRedraw = false;
            redrawMutex.release();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public boolean checkNeedsRedraw(){
        boolean result = false;
        try {
            redrawMutex.acquire();
            result = needsRedraw;
            redrawMutex.release();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return result;
    }
}
