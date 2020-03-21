package com.example.justgottagetrightouttahere_client.Views;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import com.example.justgottagetrightouttahere_client.model.GameMessageHandler;
import com.example.justgottagetrightouttahere_client.model.GameModel;
import com.example.justgottagetrightouttahere_client.model.Player;
import com.example.justgottagetrightouttahere_client.model.ResourcesMaps;
import com.example.justgottagetrightouttahere_client.network.TCPClient;

import java.util.List;

import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_X;
import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_Y;
import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_TILE_SIZE;

public class GameView extends View {
    GameModel gameModel;
    GameMessageHandler messageHandler;
    /**The size of a tile in pixels, adapted to screen size**/
    int renderTileSize = DEFAULT_TILE_SIZE;
    boolean renderTileSizeCalculated = false;
    TCPClient client = null;

    public void calculateTileSize(){
        //TODO FIXME
        int height = getMeasuredHeight();
        int width = getRootView().getWidth();
        int renderTileSize_X = width / gameModel.sizeX;
        int renderTileSize_Y = height/ gameModel.sizeY;

        renderTileSize = renderTileSize_X < renderTileSize_Y ? renderTileSize_X:renderTileSize_Y;
        //System.err.println(renderTileSize);
    }

    /**
     * Creates the game view without initializing the game model
     * @param context
     */
    public GameView(Context context) {
        super(context);
        ResourcesMaps.LoadSprites();
        gameModel = new GameModel(DEFAULT_SIZE_X,DEFAULT_SIZE_Y);

        messageHandler = new GameMessageHandler(gameModel);

        client = TCPClient.getInstance();
        if(!client.TCPClientRunning){
            Thread clientThread = new Thread(client);
            clientThread.start();
        }

        while(!client.setMessageHandler(messageHandler)){}

        //FIXME remove this (testing purpose)
        gameModel.players.add(new Player(0,0,0,0));
        gameModel.movePlayer(0,2,2);
        gameModel.players.add(new Player(1,0,1,1));
        gameModel.players.add(new Player(0,1,2,2));
        gameModel.players.add(new Player(1,1,3,3));
        gameModel.objectLayer[3][3] = 1;
        /*
        try {
            JSONObject jsonObject = new JSONObject("{\"Action\":\"TestAction\"}");
            Log.e("INFO","Action : " + jsonObject.getString("Action"));
        } catch (JSONException e) {
            e.printStackTrace();
            //{Action:"move",PosX:[val],PosY:[val],Player:[id]}
        }*/
    }
    public void send(String s){
        if(client != null){
            Log.e("DEBUG","sendmessage : "+s);
            //client.send(s);
        }else{
            Log.e("ERROR","Not connected to a game");
        }
    }

    /**
     * Creates the game view and initializes the game model with the give X and Y size
     * @param context
     * @param sizeX
     * @param sizeY
     */
    public GameView(Context context, int sizeX, int sizeY) {
        super(context);
        gameModel = new GameModel(sizeX,sizeY);
        ResourcesMaps.LoadSprites();
    }

    /**
     * Draws the game model in the given canvas
     * @param canvas
     * **/
    @Override
    public void draw(Canvas canvas){
        //FIXME find a clever way to ask for a refresh

        super.draw(canvas);
        canvas.drawColor(Color.RED);
        if(!renderTileSizeCalculated){
            calculateTileSize();
        }

        drawBlocks(canvas,gameModel.blocksLayer,gameModel.sizeX, gameModel.sizeY);
        drawPlayers(canvas,gameModel.players);
        drawObjects(canvas,gameModel.objectLayer,gameModel.sizeX, gameModel.sizeY);
        //gameModel.setRedrawDone();
        invalidate();

    }

    void drawObjects(Canvas canvas, int objects[][], int sizeX, int sizeY){
        for(int i = 0; i < sizeX;++i){
            for(int j = 0; j < sizeY;++j){
                if(objects[i][j] != 0){
                    int spriteId = ResourcesMaps.objectsSpritesMap.get(objects[i][j]);
                    drawImage(canvas,i*renderTileSize,j*renderTileSize,i*renderTileSize+renderTileSize,j*renderTileSize+renderTileSize, spriteId);
                }
            }
        }
    }

    /**
     * Draws the players in the given canvas
     * @param canvas
     */
    void drawPlayers(Canvas canvas, List<Player> players){
        for(Player p : players){
            //System.err.println("Drawing player "+p.id);

            int playerSpriteId = ResourcesMaps.playerSpritesMap.get(p.roleId);
            drawImage(canvas,p.posX*renderTileSize, p.posY*renderTileSize,p.posX*renderTileSize+renderTileSize,p.posY*renderTileSize+renderTileSize,playerSpriteId);
        }
    }

    /**
     * Draws the tile matrix in the given canvas
     * @param canvas
     */
    void drawBlocks(Canvas canvas, int matrix[][], int sizeX, int sizeY){
        for(int i = 0; i < sizeX;++i){
            for(int j = 0; j < sizeY;++j){
                int tileSpriteId = ResourcesMaps.blocksSpritesMap.get(matrix[i][j]);
                drawImage(canvas,i*renderTileSize,j*renderTileSize,i*renderTileSize+renderTileSize,j*renderTileSize+renderTileSize, tileSpriteId);
            }
        }
    }

    private void drawImage(Canvas canvas,int left,int top, int right, int bot, int imageResource){
        Paint p = new Paint();
        Bitmap b= BitmapFactory.decodeResource(getResources(), imageResource);
        Rect r = new Rect(left,top,right,bot);
        canvas.drawBitmap(b, null, r, p);
    }

}
