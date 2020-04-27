using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListElementModel
{
	public int GameId;
	public int NbPlayers;

	public GameListElementModel(int gameId, int nbPlayers) {
		GameId = gameId;
		NbPlayers = nbPlayers;
	}
}
