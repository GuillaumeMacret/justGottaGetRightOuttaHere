using System;
using System.Collections;
using System.Collections.Generic;
using SimpleJSON;
using UnityEngine;
using UnityEngine.UI;

public class LobbyManager : MonoBehaviour
{
	[Header("Roles")]
	public Sprite[] RoleCharacterSprites;
	public string[] RoleDescriptions;

	[Header("Players")]
	public LobbyPlayerController[] LobbyPlayerControllers;

	[Header("Game")]
	public Text GameIdText;
	public Button StartGameButton;

	[Header("Network")]
	public LobbyNetworkRunner Runner;
	public Image CantStartGameErrorMessageCanvas;
	public Text ErrorMessageText;

	[Header("Map")]
	public Dropdown MapsDropdown;
	public Image MapName;
	public Text MapText;

	// Private bool to change state of UI
	private bool changeMapName;
	private bool playerJoined;
	private bool playerLeft;
	private bool sceneInitialized;
	private bool showErrorMessage;
	private int playerJoinedIndex;
	private int playerLeftIndex;

    // Start is called before the first frame update
    void Start()
    {
		GameLobbyData.TotalNbRoles = RoleDescriptions.Length;
		GameIdText.text = "Game " + GameLobbyData.GameId;
		if (GameLobbyData.CreatedGame) // Game created by user
		{
			MapName.gameObject.SetActive(false);
			MapsDropdown.AddOptions(new List<string>(GameLobbyData.MapList));
			MapsDropdown.onValueChanged.AddListener(delegate { MapChanged(MapsDropdown); });
			if (!string.IsNullOrEmpty(GameLobbyData.LevelName))
				MapsDropdown.value = MapsDropdown.options.FindIndex((i) => { return i.text.Equals(GameLobbyData.LevelName); });
		}
		else // Game not created by user
		{
			MapsDropdown.gameObject.SetActive(false);
			StartGameButton.interactable = false;
			ChangeMap(GameLobbyData.MapName);
		}
		for (int i = 0; i < GameLobbyData.PlayersRoles.Length; ++i) {
			LobbyPlayerControllers[i].ChangeRole(GameLobbyData.PlayersRoles[i], RoleCharacterSprites[GameLobbyData.PlayersRoles[i]], RoleDescriptions[GameLobbyData.PlayersRoles[i]]);
		}
		// Removing change character button for other players
		for (int i = 0; i < LobbyPlayerControllers.Length; ++i) {
			if (i != GameLobbyData.PlayerId) {
				LobbyPlayerControllers[i].PreviousCharacterButton.gameObject.SetActive(false);
				LobbyPlayerControllers[i].NextCharacterButton.gameObject.SetActive(false);
			}
		}
		showErrorMessage = false;

	}

    // Update is called once per frame
    void Update()
    {
        if(changeMapName) 
		{
			MapText.text = GameLobbyData.MapName;
			changeMapName = false;
		}
		if(playerJoined) 
		{
			LobbyPlayerControllers[playerJoinedIndex].ChangeRole(GameLobbyData.PlayersRoles[playerJoinedIndex], RoleCharacterSprites[GameLobbyData.PlayersRoles[playerJoinedIndex]], RoleDescriptions[GameLobbyData.PlayersRoles[playerJoinedIndex]]);
			playerJoined = false;
		}
		if(playerLeft) 
		{
			LobbyPlayerControllers[playerLeftIndex].Reset();
			playerLeft = false;
		}
		if(showErrorMessage) 
		{
			CantStartGameErrorMessageCanvas.gameObject.SetActive(true);
		}
		else 
		{
			CantStartGameErrorMessageCanvas.gameObject.SetActive(false);
		}
    }

	public void ShowErrorMessage(string message) 
	{
		showErrorMessage = true;
		ErrorMessageText.text = message;
	}

	public void CloseErrorMessage() 
	{
		showErrorMessage = false;
	}

	// Will send a message to the server requesting a map change
	void MapChanged(Dropdown change) 
	{
		Runner.SendMapChangeRequest(change.value);
	}

	// Is called when a player joined the lobby, to update its role
	public void JoinedGame(int playerId, int[] roles) 
	{
		playerJoinedIndex = playerId;
		playerJoined = true;
	}

	// Is called when the leader has changed the map
	public void ChangeMap(string newMap) 
	{
		GameLobbyData.MapName = newMap;
		changeMapName = true;
	}

	// Is called when a player changed role
	public void ChangeRole(int playerId, int roleId) 
	{
		if(roleId < RoleCharacterSprites.Length && roleId < RoleDescriptions.Length) 
		{
			GameLobbyData.PlayersRoles[playerId] = roleId;
			LobbyPlayerControllers[playerId].ChangeRole(roleId, RoleCharacterSprites[roleId], RoleDescriptions[roleId]);
		}
	}

	public void PlayerLeftGame(int playerId) 
	{
		playerLeftIndex = playerId;
		playerLeft = true;
	}
}
