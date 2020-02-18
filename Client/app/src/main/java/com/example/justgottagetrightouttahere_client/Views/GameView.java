package com.example.justgottagetrightouttahere_client.Views;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.MotionEvent;
import android.view.View;

import androidx.constraintlayout.solver.widgets.Rectangle;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameModel;
import com.example.justgottagetrightouttahere_client.model.TilesSprites;

import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_X;
import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_Y;
import static com.example.justgottagetrightouttahere_client.Constants.Constants.TILE_SIZE;

public class GameView extends View {
    GameModel gameModel;

    /**
     * Creates the game view without initializing the game model
     * @param context
     */
    public GameView(Context context) {
        super(context);
        TilesSprites.LoadSprites();
        gameModel = new GameModel(DEFAULT_SIZE_X,DEFAULT_SIZE_Y);
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
        TilesSprites.LoadSprites();
    }

    @Override
    public void draw(Canvas canvas){
        super.draw(canvas);
        canvas.drawColor(Color.RED);
        drawMatrix(canvas);
    }

    private void drawMatrix(Canvas canvas){
        for(int i = 0; i < gameModel.gameMatrix.length;++i){
            for(int j = 0; j < gameModel.gameMatrix[i].length;++j){
                int tileId = TilesSprites.tilesSpritesMap.get(gameModel.gameMatrix[i][j]);
                drawImage(canvas,i*TILE_SIZE,j+TILE_SIZE,i*TILE_SIZE+TILE_SIZE,j*TILE_SIZE+TILE_SIZE, tileId);
            }
        }
    }



    private void drawImage(Canvas canvas,int posX,int posY, int sizeX, int sizeY, int imageRessource){
        Paint p = new Paint();
        Bitmap b= BitmapFactory.decodeResource(getResources(), imageRessource);
        //p.setColor(Color.YELLOW);
        Rect r = new Rect(posX,posY,sizeX,sizeY);
        canvas.drawBitmap(b, null, r, p);
        System.err.println(r);
    }

    public boolean onTouchEvent(MotionEvent event){
        return true;
        //TODO
    }
}
