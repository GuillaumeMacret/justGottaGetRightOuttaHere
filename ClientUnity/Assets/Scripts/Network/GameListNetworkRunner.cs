using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameListNetworkRunner : MonoBehaviour
{
	public GameListMessageHandler handler;
	public float TimeBetweenRefreshRequests = 5f;
	public float TimeoutThreshold = 10f;

	public Image ConnectingToServerPanel;
	public Image TimeoutMessagePanel;

	private float lastRefresh;
	private float waitingForConnection;
	private bool timeoutMessage;
	private bool isConnected;
    // Start is called before the first frame update
    void Start()
    {
		ConnectToServer();
		lastRefresh = 0f;
		waitingForConnection = 0f;
		TimeBetweenRefreshRequests = 1f;
		isConnected = false;
		timeoutMessage = false;
	}

	void Update() {
		isConnected = TCPClient.IsSocketConnected();
		if (!isConnected) 
		{ 
			waitingForConnection += Time.deltaTime;
			ConnectingToServerPanel.gameObject.SetActive(true);
		}
		else 
		{
			lastRefresh += Time.deltaTime;
			ConnectingToServerPanel.gameObject.SetActive(false);
			if(lastRefresh > TimeBetweenRefreshRequests) 
			{
				SendGameListRequest();
				TimeBetweenRefreshRequests = 5f;
			}
		}
		if (waitingForConnection > TimeoutThreshold && !timeoutMessage) 
		{
			timeoutMessage = true;
			TimeoutMessagePanel.gameObject.SetActive(true);
		}
	}

	private void ConnectToServer() 
	{
		TCPClient.ConnectIfNotConnected();
		TCPClient.SetMessageHandler(handler);
		TimeoutMessagePanel.gameObject.SetActive(false);
	}

	public void RetryToConnect() 
	{
		timeoutMessage = false;
		waitingForConnection = 0f;
		isConnected = false;
		ConnectToServer();
	}

	public void SendGameListRequest() 
	{
		string messageToSend = MessageBuilders.BuildGameListMessage();
		if (messageToSend != "") 
		{
			TCPClient.SendMessage(messageToSend);
			lastRefresh = 0f;
		}
	}

	public void SendGameCreateRequest() 
	{
		string messageToSend = MessageBuilders.BuildCreateGameMessage();
		if(messageToSend != "") 
		{
			TCPClient.SendMessage(messageToSend);
		}
	}

	public void CloseTimeoutMessage() 
	{
		timeoutMessage = false;
		waitingForConnection = 0f;
	}
}
