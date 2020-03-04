package com.example.justgottagetrightouttahere_client.Activities;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.Views.GameView;

public class GameActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(new GameView(this));
        setContentView(R.layout.game_activity);
    }
}
