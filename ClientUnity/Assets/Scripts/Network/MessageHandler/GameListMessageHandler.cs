using SimpleJSON;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListMessageHandler : MonoBehaviour, IMessageHandler
{
	public GameListCreator gameListCreator;
	public GameSceneManager sceneManager;

	public void Handle(string JSONString) {
		ActionJson action = JsonUtility.FromJson<ActionJson>(JSONString);
		Debug.Log("Action recieved : " + action.Action);
		switch (action.Action) {
			case "gameList":
				JSONNode gameListJson = JSON.Parse(JSONString);
				JSONArray games = gameListJson["Games"].AsArray;
				List<GameListElementModel> gameList = new List<GameListElementModel>();
				for(int i = 0; i < games.Count; ++i) {
					gameList.Add(new GameListElementModel(games[i]["id"], games[i]["nbPlayers"]));
				}
				gameListCreator.SetGameListAndUpdate(gameList);
				break;
			case "joinedGame":
				JSONNode joinedGame = JSON.Parse(JSONString);
				int gameId = joinedGame["GameId"];
				int playerId = joinedGame["PlayerId"];
				string map = joinedGame["Map"];
				JSONArray playersJson = joinedGame["Players"].AsArray;
				int[] players = new int[playersJson.Count];
				for (int i = 0; i < playersJson.Count; ++i)
					players[i] = playersJson[i];
				sceneManager.ToLobbyScene(gameId, playerId, players, map);
				break;
			case "cantJoinGame":
				JSONNode cantJoin = JSON.Parse(JSONString);
				gameId = cantJoin["GameId"];
				string errorMessage = cantJoin["MoreInfo"];
				//TODO : Display the error message
				break;
			default:
				Debug.LogError("Can't handle this action : " + action.Action);
				break;
		}
	}

	// Start is called before the first frame update
	void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
