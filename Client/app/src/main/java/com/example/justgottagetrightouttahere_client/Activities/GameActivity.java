package com.example.justgottagetrightouttahere_client.Activities;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.Toast;

import com.example.justgottagetrightouttahere_client.R;

public class GameActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(new GameView(this));
        setContentView(R.layout.game_activity);

    }

    private float x1,x2,y1,y2;
    static final int MIN_SWIPE_DISTANCE = 150;

    @Override
    public boolean onTouchEvent(MotionEvent event){
        switch(event.getAction()){
            case MotionEvent.ACTION_DOWN:
                x1 = event.getX();
                y1 =event.getY();
                break;
            case MotionEvent.ACTION_UP:
                x2 = event.getX();
                y2 =event.getY();
                float deltaX = x2 - x1, deltaY = y2 - y1;
                if(deltaX > MIN_SWIPE_DISTANCE){
                    Toast.makeText(this,"left2right swipe",Toast.LENGTH_SHORT).show();
                    findViewById(R.id.gameBoardFragment);
                }else if(deltaX < -MIN_SWIPE_DISTANCE) {
                    Toast.makeText(this,"right2left swipe",Toast.LENGTH_SHORT).show();
                }else if(deltaY > MIN_SWIPE_DISTANCE){
                    Toast.makeText(this,"top2bot swipe",Toast.LENGTH_SHORT).show();
                }else if(deltaY < MIN_SWIPE_DISTANCE) {
                    Toast.makeText(this,"bot2top swipe",Toast.LENGTH_SHORT).show();
                }else{
                    //Screen tap
                    Toast.makeText(this,"ScreenTap",Toast.LENGTH_SHORT).show();
                }
                break;
            default:
                break;
        }
        return super.onTouchEvent(event);
    }
}
