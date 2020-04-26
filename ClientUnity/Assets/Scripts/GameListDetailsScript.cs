using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameListDetailsScript : MonoBehaviour
{
	public Text GameIDText;
	public int NbPlayers = 0;
	public Button JoinButton;

	public GameListDetailsScript(string gameID, int nbPlayers) {
		GameIDText.text = gameID;
		NbPlayers = nbPlayers;
	}
}
