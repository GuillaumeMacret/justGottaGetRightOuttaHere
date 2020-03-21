package com.example.justgottagetrightouttahere_client.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.Player;

public class LobbyActivity extends AppCompatActivity {
    protected int currentNbPlayers = 0;

    protected Player[] players;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.lobby_activity);

        players = new Player[4];


        Button button = findViewById(R.id.start_game_button);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(),GameActivity.class);
                startActivity(intent);
            }
        });
    }
}
