using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public int xPos, yPos, id;

    private Animator animator;

    //FIXME public for test only
    public List<Vector2> targetPositions;

    private void Awake()
    {
        animator = GetComponent<Animator>();
    }

    private void MoveTowardsNextDestination()
    {
        Debug.Log("Player " + id + " Moving toward " + targetPositions[0]);
    }

    private void Update()
    {
        if(targetPositions.Count > 0)
        {
            Vector2 direction = new Vector2(transform.position.x, transform.position.y) - targetPositions[0];
            MoveTowardsNextDestination();
            animator.SetFloat("MoveX", direction.x);
            animator.SetFloat("MoveY", direction.y);
        }

    }

    public Player(int xPos, int yPos, int id)
    {
        this.xPos = xPos;
        this.yPos = yPos;
        this.id = id;
    }

    public void AddDestination(int xPos, int yPos)
    {
        targetPositions.Add(new Vector2(xPos, yPos));
    }
}
