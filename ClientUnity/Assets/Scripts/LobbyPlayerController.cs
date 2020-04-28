using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LobbyPlayerController : MonoBehaviour
{
	public LobbyNetworkRunner lobbyNetworkRunner;

	public Sprite DefaultCharacterSprite;
	public Button ChangeCharacterButton;
	public Text PlayerIdText;
	public Text AbilityText;
	public Image CharacterImage;
	
	private int playerId;
	private int currentRole;
	private string ability;
	private bool changeRole;
	private Sprite characterSprite;

    void Awake()
    {
		ability = "Ability:";
		characterSprite = DefaultCharacterSprite;
		changeRole = true;
    }

	void Start() {
		ChangeCharacterButton.onClick.AddListener(ChangeCharacter);
	}

	// Update is called once per frame
	void Update()
    {
        if(changeRole) 
		{
			if (characterSprite != null)
				CharacterImage.sprite = characterSprite;
			else
				CharacterImage.sprite = DefaultCharacterSprite;
			AbilityText.text = ability;
			changeRole = false;
			//lastUpdate = 0f;
		}
    }

	void ChangeCharacter() 
	{
		if (lobbyNetworkRunner != null)
			lobbyNetworkRunner.SendRoleChangeRequest((currentRole + 1) % GameLobbyData.TotalNbRoles);
	}

	public void ChangeRole(int roleId, Sprite newCharacterSprite, string newAbility) 
	{
		currentRole = roleId;
		characterSprite = newCharacterSprite;
		ability = newAbility;
		changeRole = true;
	}

	public void Reset() 
	{
		characterSprite = DefaultCharacterSprite;
		ability = "Ability:";
		changeRole = true;
	}
}
