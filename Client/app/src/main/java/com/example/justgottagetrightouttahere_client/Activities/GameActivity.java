package com.example.justgottagetrightouttahere_client.Activities;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.Toast;

import com.example.justgottagetrightouttahere_client.Constants.MessageTemplates;
import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.network.TCPClient;

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
                    String message = MessageTemplates.createMoveActionMessage("right");
                    TCPClient.getInstance().sendThreaded(message);
                }else if(deltaX < -MIN_SWIPE_DISTANCE) {
                    String message = MessageTemplates.createMoveActionMessage("left");
                    TCPClient.getInstance().sendThreaded(message);
                }else if(deltaY > MIN_SWIPE_DISTANCE){
                    String message = MessageTemplates.createMoveActionMessage("down");
                    TCPClient.getInstance().sendThreaded(message);
                }else if(deltaY < MIN_SWIPE_DISTANCE) {
                    String message = MessageTemplates.createMoveActionMessage("up");
                    TCPClient.getInstance().sendThreaded(message);
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
