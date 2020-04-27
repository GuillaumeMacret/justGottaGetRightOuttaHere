using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameListItemController : MonoBehaviour
{
	public Text GameIDText;
	public Text NbPlayersText;
	public Button JoinButton;

	public void JoinGame() 
	{
		string message = MessageBuilders.BuildJoinGameMessage(int.Parse(GameIDText.text));
		if (message != "")
			TCPClient.SendMessage(message);
	}
}
