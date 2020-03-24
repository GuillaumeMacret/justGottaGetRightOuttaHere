package com.example.justgottagetrightouttahere_client.Activities;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.example.justgottagetrightouttahere_client.Constants.MessageTemplates;
import com.example.justgottagetrightouttahere_client.Fragments.LobbyPlayerFragment;
import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameInfo;
import com.example.justgottagetrightouttahere_client.model.GameMessageHandler;
import com.example.justgottagetrightouttahere_client.model.Player;
import com.example.justgottagetrightouttahere_client.network.TCPClient;

public class LobbyActivity extends AppCompatActivity {
    private boolean created = false;
    protected int currentNbPlayers = 0;
    protected GameInfo gameInfo;
    protected Player[] players;
    private LobbyPlayerFragment[] lobbyPlayerFragments;

    GameMessageHandler messageHandler;
    TCPClient client;

    @SuppressLint("SetTextI18n")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.lobby_activity);

        players = new Player[4];
        Intent intent = getIntent();

        // Fragment retrieving
        lobbyPlayerFragments = new LobbyPlayerFragment[4];
        lobbyPlayerFragments[0] = (LobbyPlayerFragment)this.getSupportFragmentManager().findFragmentById(R.id.player1_fragment);
        lobbyPlayerFragments[1] = (LobbyPlayerFragment)this.getSupportFragmentManager().findFragmentById(R.id.player2_fragment);
        lobbyPlayerFragments[2] = (LobbyPlayerFragment)this.getSupportFragmentManager().findFragmentById(R.id.player3_fragment);
        lobbyPlayerFragments[3] = (LobbyPlayerFragment)this.getSupportFragmentManager().findFragmentById(R.id.player4_fragment);

        // Message receiving
        messageHandler = new GameMessageHandler();
        messageHandler.lobbyActivity = this;

        client = TCPClient.getInstance();
        if(!client.TCPClientRunning){
            Thread clientThread = new Thread(client);
            clientThread.start();
        }
        while(!client.setMessageHandler(messageHandler)) {}

        // View initialization
        created = intent.getBooleanExtra("created", false);
        int gameId = intent.getIntExtra("gameId", 0);
        currentNbPlayers = intent.getIntExtra("nbPlayers", 0);
        gameInfo = new GameInfo(gameId, currentNbPlayers);

        // Change activity if the client has created or not the lobby
        if(created) {
            final String[] maps = intent.getStringArrayExtra("maps");
            //Remove textview for map name since we have the spinner
            TextView mapTextView = (TextView)findViewById(R.id.mapTextView);
            ((ViewGroup)mapTextView.getParent()).removeView(mapTextView);

            Spinner mapSpinner = (Spinner)findViewById(R.id.mapSpinner);
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, maps);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            mapSpinner.setAdapter(adapter);
            mapSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    if(maps[position] != null) {
                        String message = MessageTemplates.createChangeMapMessage(maps[position]);
                        TCPClient.sendThreaded(message);
                    }
                }
                @Override
                public void onNothingSelected(AdapterView<?> parent) {}
            });

            players[0] = new Player(0, 0);
            lobbyPlayerFragments[0].updatePlayerImageAndText(players[0]);
        }
        else {
            int[] roles = intent.getIntArrayExtra("roles");
            String map = intent.getStringExtra("map");

            for(int i = 0; i < currentNbPlayers; ++i) {
                if(roles != null) {
                    players[i] = new Player(i, roles[i]);
                    lobbyPlayerFragments[i].updatePlayerImageAndText(players[i]);
                }
            }
            for(int i = 0; i < 4; ++i) {
                if(i != currentNbPlayers - 1)
                    lobbyPlayerFragments[i].removeChangeCharacterButton();
            }

            Spinner mapSpinner = (Spinner)findViewById(R.id.mapSpinner);
            ((ViewGroup)mapSpinner.getParent()).removeView(mapSpinner);
            TextView mapTextView = (TextView)findViewById(R.id.mapTextView);
            mapTextView.setText(map);
        }
        currentNbPlayers++;

        TextView gameIdTextView = (TextView) findViewById(R.id.game_id_text_view);
        gameIdTextView.setText(Integer.toString(gameInfo.id));
        Button button = findViewById(R.id.start_game_button);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(created) {
                    String message = MessageTemplates.createStartGameMessage();
                    TCPClient.sendThreaded(message);
                    //Intent intent = new Intent(getApplicationContext(),GameActivity.class);
                    //startActivity(intent);
                }
            }
        });
    }

    public void mapChange(String newMap) {
        if(!created) {
            TextView mapTextView = (TextView)findViewById(R.id.mapTextView);
            mapTextView.setText(newMap);
        }
    }

    public void changeCharacter(int playerId, int role) {
        lobbyPlayerFragments[playerId].player.roleId = role;
        lobbyPlayerFragments[playerId].updatePlayerImageAndText();
    }

    public void cantStartGame(String errorMessage) {
        Toast toast = Toast.makeText(getApplicationContext(), "Could not start game: " + errorMessage, Toast.LENGTH_SHORT);
        toast.show();
    }

    public void playerJoined(int playerId) {
        lobbyPlayerFragments[playerId].updatePlayerImageAndText(new Player(playerId, 0));
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        finish();
    }
}
