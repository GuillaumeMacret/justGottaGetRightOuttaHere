package com.example.justgottagetrightouttahere_client.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.justgottagetrightouttahere_client.Adapters.GameInfoAdapter;
import com.example.justgottagetrightouttahere_client.Constants.MessageTemplates;
import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameInfo;
import com.example.justgottagetrightouttahere_client.model.GameMessageHandler;
import com.example.justgottagetrightouttahere_client.network.TCPClient;

import java.util.ArrayList;

public class GameListActivity extends AppCompatActivity {

    ListView gameListView;
    Button createGameButton;
    Button refreshListButton;
    ArrayList<GameInfo> gameList;
    GameMessageHandler messageHandler;
    TCPClient client;

    GameInfoAdapter gameInfoAdapter;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game_list_activity);

        // Initialize elements
        gameListView = (ListView)findViewById(R.id.game_listview);
        gameList = new ArrayList<>();

        gameInfoAdapter = new GameInfoAdapter(gameList, getApplicationContext());
        gameListView.setAdapter(gameInfoAdapter);

        // Receive game list
        messageHandler = new GameMessageHandler();
        messageHandler.gameList = gameList;
        messageHandler.gameListActivity = this;

        client = TCPClient.getInstance();
        if(!client.TCPClientRunning){
            Thread clientThread = new Thread(client);
            clientThread.start();
        }
        while(!client.setMessageHandler(messageHandler)) {}
        /////////

        String message = MessageTemplates.createGameListMessage();
        TCPClient.sendThreaded(message);

        // Click action for games
        gameListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                messageHandler.tryingToJoinGame = true;
                String message = MessageTemplates.createJoinGameMessage(gameList.get(position).id);
                TCPClient.sendThreaded(message);
            }
        });

        // Click action for creating game
        createGameButton = (Button)findViewById(R.id.create_game_button);
        createGameButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String message = MessageTemplates.createGameMessage();
                TCPClient.sendThreaded(message);
            }
        });

        // Click action for refreshing list
        refreshListButton = (Button)findViewById(R.id.refresh_game_list_button);
        refreshListButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String message = MessageTemplates.createGameListMessage();
                TCPClient.sendThreaded(message);
            }
        });
    }

    public GameInfoAdapter getGameInfoAdapter() {
        return gameInfoAdapter;
    }

    public void gameCreated(int gameId, String[] maps) {
        Intent intent = new Intent(getApplicationContext(), LobbyActivity.class);
        intent.putExtra("created", true);
        intent.putExtra("gameId", gameId);
        intent.putExtra("nbPlayers", 0);
        intent.putExtra("maps", maps);
        startActivity(intent);
    }

    public void joinGame(int gameId, int nbPlayers, int[] roles, String map) {
        Intent intent = new Intent(getApplicationContext(), LobbyActivity.class);
        intent.putExtra("created", false);
        intent.putExtra("gameId", gameId);
        intent.putExtra("nbPlayers", nbPlayers);
        intent.putExtra("roles", roles);
        intent.putExtra("map", map);
        startActivity(intent);
    }

    public void cantJoinGame(String errorMessage) {
        Toast toast = Toast.makeText(getApplicationContext(), "Could not join game: " + errorMessage, Toast.LENGTH_SHORT);
        toast.show();
    }

}
