using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameSceneManager : MonoBehaviour
{
    public void ToGameListScene() {
		SceneManager.LoadScene("GameList");
	}
	
	public void ToLobbyScene(int gameId, int playerId, int[] players, string map) {
		GameLobbyData.GameId = gameId;
		GameLobbyData.PlayerId = playerId;
		GameLobbyData.Players = players;
		GameLobbyData.MapName = map;
		SceneManager.LoadScene("Lobby");
	}
}
