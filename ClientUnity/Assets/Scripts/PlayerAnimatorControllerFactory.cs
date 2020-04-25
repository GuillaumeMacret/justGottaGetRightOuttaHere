using System.Collections;
using System.Collections.Generic;
using UnityEditor.Animations;
using UnityEngine;

public class PlayerAnimatorControllerFactory : MonoBehaviour
{
    public const int ROLE_PHANTOM = 1;
    public const int ROLE_KNIGHT = 2;
    public const int ROLE_WAR = 3;
    public const int ROLE_PUMKIN = 4;
    public const int ROLE_MAGE = 5;
    public const int ROLE_ELF = 6;

    public readonly string[] roleAnimationPrefixes = {"phantom","knight","war","pumkin","mage","elf" };

    public List<AnimatorController> playerAnimators;

    public AnimatorController GetAnimatorController(int id)
    {
        if (id > playerAnimators.Count - 1)
        {
            Debug.LogWarning("No animator found for id : " + id + ", returning the default one");
            return playerAnimators[0];
        }
        return playerAnimators[id];
    }
}
