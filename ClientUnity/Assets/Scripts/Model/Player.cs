using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public int id;
    public int speed = 10;

    private Animator animator;

    //FIXME public for test only
    public List<Vector3> targetPositions;

    private void Awake()
    {
        animator = GetComponent<Animator>();
    }

    private void MoveTowardsNextDestination()
    {
        Debug.Log("Player " + id + " Moving toward " + targetPositions[0]);
        float step = speed * Time.deltaTime;
        transform.position = Vector3.MoveTowards(transform.position, targetPositions[0], step);

        if(transform.position == targetPositions[0])
        {
            targetPositions.RemoveAt(0);
        }
    }

    private void Update()
    {
        if(targetPositions.Count > 0)
        {
            Vector3 direction = targetPositions[0] - transform.position;
            MoveTowardsNextDestination();
            animator.SetFloat("MoveX", direction.x);
            animator.SetFloat("MoveY", direction.y);
            animator.SetBool("Moving", true);
        }
        else
        {
            animator.SetBool("Moving", false); 
        }

    }

    public void AddDestination(int xPos, int yPos)
    {
        targetPositions.Add(new Vector3(xPos, yPos, 0));
    }
}
