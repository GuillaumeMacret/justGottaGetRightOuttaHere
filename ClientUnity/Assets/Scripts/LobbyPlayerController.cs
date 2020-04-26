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

    // Start is called before the first frame update
    void Start()
    {
		ability = "Ability:";
		characterSprite = DefaultCharacterSprite;
		ChangeCharacterButton.onClick.AddListener(ChangeCharacter);
		changeRole = true;
    }

    // Update is called once per frame
    void Update()
    {
        if(changeRole) {
			if (characterSprite != null)
				CharacterImage.sprite = characterSprite;
			else
				CharacterImage.sprite = DefaultCharacterSprite;
			AbilityText.text = ability;
			changeRole = false;
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
}
