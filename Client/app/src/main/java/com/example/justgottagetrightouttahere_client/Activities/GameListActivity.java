package com.example.justgottagetrightouttahere_client.Activities;

import android.content.Intent;
import android.os.Bundle;
import android.os.PersistableBundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.example.justgottagetrightouttahere_client.Adapters.GameInfoAdapter;
import com.example.justgottagetrightouttahere_client.R;
import com.example.justgottagetrightouttahere_client.model.GameInfo;

import java.util.ArrayList;

public class GameListActivity extends AppCompatActivity {

    ListView gameListView;
    ArrayList<GameInfo> gameList;

    private GameInfoAdapter gameInfoAdapter;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game_list_activity);

        gameListView = (ListView)findViewById(R.id.game_listview);
        gameList = new ArrayList<>();

        gameList.add(new GameInfo("dfa5g", 2));
        gameList.add(new GameInfo("fzlkf", 4));
        gameList.add(new GameInfo("rgjkslkjg", 1));
        gameList.add(new GameInfo("sdlksj", 3));
        gameList.add(new GameInfo("sldgjks", 2));
        gameList.add(new GameInfo("dfa5g", 2));
        gameList.add(new GameInfo("fzlkf", 4));
        gameList.add(new GameInfo("rgjkslkjg", 1));
        gameList.add(new GameInfo("sdlksj", 3));
        gameList.add(new GameInfo("sldgjks", 2));

        gameInfoAdapter = new GameInfoAdapter(gameList, getApplicationContext());
        gameListView.setAdapter(gameInfoAdapter);

        gameListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                //TODO: Send a message to the server saying you want to join a game, the server will answer a "joinedGame" or "cantJoinGame" message
                if(true)/*Le message est OK)*/ {
                    Intent intent = new Intent(getApplicationContext(), LobbyActivity.class);
                    intent.putExtra("gameId", gameList.get(position).id);
                    intent.putExtra("nbPlayers", gameList.get(position).nbPlayers);
                    //TODO: Put extra for each player intent.putExtra("playerID...",playerID)...
                    startActivity(intent);
                }
                else {
                    Toast toast = Toast.makeText(getApplicationContext(), "Message d'erreur", Toast.LENGTH_SHORT);
                    toast.show();
                }
            }
        });
    }


}
