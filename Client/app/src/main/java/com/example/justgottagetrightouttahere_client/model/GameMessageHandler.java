package com.example.justgottagetrightouttahere_client.model;

import android.util.Log;

import com.example.justgottagetrightouttahere_client.Activities.GameListActivity;
import com.example.justgottagetrightouttahere_client.Activities.LobbyActivity;
import com.example.justgottagetrightouttahere_client.Adapters.GameInfoAdapter;
import com.example.justgottagetrightouttahere_client.Fragments.GameboardFragment;
import com.example.justgottagetrightouttahere_client.network.MessageHandler;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

public class GameMessageHandler implements MessageHandler {

    GameModel model;

    // For GameListActivity
    public ArrayList<GameInfo> gameList;
    public GameListActivity gameListActivity;
    public boolean tryingToJoinGame = false;

    // For LobbyActivity
    public LobbyActivity lobbyActivity;

    public GameMessageHandler(){
        this.model = GameModel.getInstance();
    }
    /**
     * Handles the message in the game board context
     * @param s
     */
    @Override
    public void handle(String s){
        System.err.println("Handling message < "+s+" >");
        try {
            JSONObject jsonObject = new JSONObject(s);
            Log.e("INFO","Action : "+jsonObject.getString("Action"));

            switch (jsonObject.getString("Action")){
                case "gameList":
                    loadGameList(jsonObject);
                    break;
                case "createdGame":
                    createGame(jsonObject);
                    break;
                case "joinedGame":
                    joinGame(jsonObject);
                    break;
                case "cantJoinGame":
                    cantJoinGame(jsonObject);
                    break;
                case "changedMap":
                    changedMap(jsonObject);
                    break;
                case "roleChange":
                    roleChange(jsonObject);
                    break;
                case "cantChangeRole":
                    //TODO
                    break;
                case "cantStartGame":
                    cantStartGame(jsonObject);
                    break;
                case "move":
                    movePlayer(jsonObject);
                    break;
                case "action":
                    updateMap(jsonObject);
                    break;
                case "loadLevel":
                    loadLevel(jsonObject);
                    break;
                default:
                    Log.e("ERROR","Game handler can't handle action "+jsonObject.getString("Action"));
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void loadGameList(JSONObject jsonObject) {
        try {
            if(gameList == null)
                gameList = new ArrayList<>();
            else
                gameList.clear();
            JSONArray jsonGamesArray = jsonObject.getJSONArray("Games");
            int arrayLength = jsonGamesArray.length();
            for(int i = 0; i < arrayLength; ++i){
                JSONObject jsonGame = jsonGamesArray.getJSONObject(i);
                gameList.add(new GameInfo(jsonGame.getInt("id"), jsonGame.getInt("nbPlayers")));
            }
            if(gameListActivity != null) {
                gameListActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        gameListActivity.getGameInfoAdapter().notifyDataSetChanged();
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void createGame(JSONObject jsonObject) {
        try {
            final int gameId = jsonObject.getInt("GameId");
            JSONArray jsonMaps = jsonObject.getJSONArray("MapList");
            final String[] maps = new String[jsonMaps.length()];
            for(int i = 0; i < maps.length; ++i) {
                maps[i] = jsonMaps.getString(i);
            }
            if(gameListActivity != null) {
                gameListActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        gameListActivity.gameCreated(gameId, maps);
                    }
                });
            }
        }
        catch(JSONException e) {
            e.printStackTrace();
        }
    }

    void joinGame(JSONObject jsonObject) {
        try {
            final int gameId = jsonObject.getInt("GameId");
            final int playerId = jsonObject.getInt("PlayerId");
            JSONArray jsonRoles = jsonObject.getJSONArray("PlayersRoles");
            final int nbPlayers = jsonRoles.length();
            final int[] playerRoles = new int[nbPlayers];
            for(int i = 0; i < nbPlayers; ++i)
                playerRoles[i] = jsonRoles.getInt(i);
            final String map = jsonObject.getString("Map");
            if(tryingToJoinGame && gameListActivity != null) {
                gameListActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        gameListActivity.joinGame(gameId, nbPlayers, playerRoles, map);
                    }
                });
                tryingToJoinGame = false;
            }
            else if(lobbyActivity != null) {
                lobbyActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        lobbyActivity.playerJoined(playerId, playerRoles[nbPlayers - 1]);
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void cantJoinGame(JSONObject jsonObject) {
        try {
            final String errorMessage = jsonObject.getString("MoreInfo");
            if(gameListActivity != null) {
                gameListActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        gameListActivity.cantJoinGame(errorMessage);
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void changedMap(JSONObject jsonObject) {
        try {
            final String newMap = jsonObject.getString("Map");
            if(lobbyActivity != null) {
                lobbyActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        lobbyActivity.mapChange(newMap);
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void roleChange(JSONObject jsonObject) {
        try {
            final int playerId = jsonObject.getInt("PlayerId");
            final int role = jsonObject.getInt("RoleId");
            if(lobbyActivity != null) {
                lobbyActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        lobbyActivity.changeCharacter(playerId, role);
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void cantStartGame(JSONObject jsonObject) {
        try {
            final String errorMessage = jsonObject.getString("MoreInfo");
            if(lobbyActivity != null) {
                lobbyActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        lobbyActivity.cantStartGame(errorMessage);
                    }
                });
            }
        }
        catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void movePlayer(JSONObject jsonObject){
        try {
            int playerId = jsonObject.getInt("Player");
            int posX = jsonObject.getInt("PosX");
            int posY = jsonObject.getInt("PosY");
            model.movePlayer(playerId,posX,posY);

            updateMap(jsonObject);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void updateMap(JSONObject jsonObject){
        List<Tile> tilesToChange = new ArrayList<>();
        try {
            JSONArray jsonTilesArray = jsonObject.getJSONArray("Changes");
            int arrayLength = jsonTilesArray.length();
            for(int i = 0; i < arrayLength;++i){
                JSONObject jsonTile = jsonTilesArray.getJSONObject(i);
                tilesToChange.add(new Tile(jsonTile.getInt("xPos"),jsonTile.getInt("yPos"),jsonTile.getInt("value")));
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        model.updateBlocksLayer(tilesToChange);
    }

    public void loadLevel(JSONObject jsonObject){
        try {
            int arrayHeight, arrayWidth;
            int blocksLayer[][] = new int[0][0];

            model.levelName = jsonObject.getString("Name");
            model.startTime = System.currentTimeMillis();

            /*Loading blocks*/
            JSONArray map = jsonObject.getJSONArray("Blocks");
            arrayHeight = map.length();
            for(int i = 0; i < arrayHeight; ++i){
                JSONArray line = map.getJSONArray(i);
                arrayWidth = line.length();
                if(i == 0){
                    blocksLayer = new int[arrayWidth][arrayHeight];
                    model.sizeX = arrayWidth;
                    model.sizeY = arrayHeight;
                }

                for(int j = 0; j < arrayWidth; ++j){
                    blocksLayer[j][i] = line.getInt(j);
                }
            }

            /*Loading Objects*/

            model.objectLayer = new int[model.sizeX][model.sizeY];

            map = jsonObject.getJSONArray("Objects");
            for(int i = 0; i < map.length(); ++i){
                JSONArray line = map.getJSONArray(i);
                arrayWidth = line.length();
                for(int j = 0; j < arrayWidth; ++j){
                    model.objectLayer[j][i] = line.getInt(j);
                }
            }

            model.loadLevel(blocksLayer);

            /*Loading players*/
            model.players = new ArrayList<>();
            map = jsonObject.getJSONArray("Players");
            for(int i = 0; i < map.length(); ++i){
                JSONObject JsonPlayers = map.getJSONObject(i);
                model.players.add(new Player());
                model.players.get(i).posX = JsonPlayers.getInt("xPos");
                model.players.get(i).posY = JsonPlayers.getInt("yPos");
                model.players.get(i).id = i;
                model.players.get(i).roleId = i;
            }

            lobbyActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    lobbyActivity.startGameActivity();
                }
            });
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
