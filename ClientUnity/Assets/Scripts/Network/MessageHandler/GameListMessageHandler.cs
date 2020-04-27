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
				JSONArray playersRolesJson = joinedGame["PlayersRoles"].AsArray;
				int[] playersRoles = new int[playersRolesJson.Count];
				for (int i = 0; i < playersRolesJson.Count; ++i)
					playersRoles[i] = playersRolesJson[i];
				sceneManager.ToLobbyScene(gameId, playerId, playersRoles, map);
				break;

			case "cantJoinGame":
				JSONNode cantJoin = JSON.Parse(JSONString);
				gameId = cantJoin["GameId"];
				string errorMessage = cantJoin["MoreInfo"];
				//TODO : Display the error message
				break;

			case "createdGame":
				JSONNode createdGame = JSON.Parse(JSONString);
				gameId = createdGame["GameId"];
				JSONArray mapsJson = createdGame["MapList"].AsArray;
				string[] mapList = new string[mapsJson.Count];
				playersRoles = new int[1];
				playersRoles[0] = 0;
				for (int i = 0; i < mapsJson.Count; ++i)
					mapList[i] = mapsJson[i];
				sceneManager.ToLobbyScene(gameId, playersRoles, mapList);
				break;

			default:
				Debug.LogError("Can't handle this action : " + action.Action);
				break;
		}
	}
}
