package com.example.justgottagetrightouttahere_client.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameInfo;
import com.example.justgottagetrightouttahere_client.model.Player;

public class LobbyActivity extends AppCompatActivity {
    protected int currentNbPlayers = 0;
    protected GameInfo gameInfo;
    protected Player[] players;
    private TextView gameIdTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.lobby_activity);

        players = new Player[4];
        Intent intent = getIntent();
        String gameId = intent.getStringExtra("gameId");
        int nbPlayers = intent.getIntExtra("nbPlayers", 0);
        if(gameId != null && nbPlayers != 0) {
            gameInfo = new GameInfo(gameId, nbPlayers);
            //TODO: Load player fragments with data
        }
        else {
            //TODO: Error : data retrieved (gameId or nbPlayers) is incorrect
        }
        gameIdTextView = (TextView) findViewById(R.id.game_id_text_view);
        gameIdTextView.setText(gameInfo.id);
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
