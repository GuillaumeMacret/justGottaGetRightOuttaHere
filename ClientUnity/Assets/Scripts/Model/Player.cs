using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public const int ROLE_PHANTOM = 1;

    public int id;
    public int speed = 10;
    public List<AudioClip> stepSounds;
    public float pitchRangeDown = .8f;
    public float pitchRangeUp = 1.2f;

    private Animator m_animator;
    private AudioSource m_audioSource;
    private SpriteRenderer m_SpriteRenderer;

    private Vector3 lastDirection;
    private int m_facing;
    private const int UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4;
    public List<Vector3> targetPositions;

    private void Awake()
    {
        m_animator = GetComponent<Animator>();
        m_audioSource = GetComponent<AudioSource>();
        m_SpriteRenderer = GetComponent<SpriteRenderer>();
    }

    private void MoveTowardsNextDestination()
    {
        Debug.Log("Player " + id + " Moving toward " + targetPositions[0]);
        float step = speed * Time.deltaTime;

        Vector3 movementVector = new Vector3(targetPositions[0].x, targetPositions[0].y, 0);
        transform.position = Vector3.MoveTowards(transform.position, movementVector, step);
        m_facing = (int) targetPositions[0].z;

        if (transform.position.x == targetPositions[0].x && transform.position.y == targetPositions[0].y)
        {
            targetPositions.RemoveAt(0);
        }
    }

    public void SetFacing(int x, int y)
    {

        m_animator.SetFloat("MoveX", x);
        m_animator.SetFloat("MoveY", y);
    }

    private void Update()
    {
        m_SpriteRenderer.enabled = true;
        speed = 10;
        if (targetPositions.Count > 0)
        {
            if (transform.position.x == targetPositions[0].x && transform.position.y == targetPositions[0].y)
            {
                targetPositions.RemoveAt(0);
                return;
            }
            else
            {
                Vector3 from = new Vector3(transform.position.x, transform.position.y, 0);
                Vector3 to = new Vector3(targetPositions[0].x, targetPositions[0].y, 0);
                Vector3 direction = to - from;
                if (Mathf.Abs(direction.x) > .5f)
                {
                    m_animator.SetFloat("MoveX", direction.x);
                    m_animator.SetFloat("MoveY", 0);
                }
                else if(Mathf.Abs(direction.y) > .5f)
                {
                    m_animator.SetFloat("MoveX", 0);
                    m_animator.SetFloat("MoveY", direction.y);
                }
                if (direction.magnitude > 3)
                {
                    Debug.Log("Big movement detected, going sneaky mode");
                    speed = 100;
                    m_SpriteRenderer.enabled = false;
                }

                MoveTowardsNextDestination();
                m_animator.SetBool("Moving", true);
                if (!m_audioSource.isPlaying && id != ROLE_PHANTOM)
                {
                    m_audioSource.pitch = Random.Range(pitchRangeDown, pitchRangeUp);
                    m_audioSource.PlayOneShot(stepSounds[Random.Range(0, stepSounds.Count)]);
                }
            }
        }
        else
        {
            m_animator.SetBool("Moving", false);
        }
        switch (m_facing)
        {
            case 0:
                break;
            case UP:
                SetFacing(0, 1);
                break;
            case DOWN:
                SetFacing(0, -1);
                break;
            case LEFT:
                SetFacing(-1, 0);
                break;
            case RIGHT:
                SetFacing(1, 0);
                break;
        }
        /*
        m_animator.SetFloat("MoveX", lastDirection.x);
        m_animator.SetFloat("MoveY", lastDirection.y);
        */

    }

    public void AddDestination(int xPos, int yPos, int facing)
    {
        targetPositions.Add(new Vector3(xPos, yPos, facing));
    }

    public void SetAnimatorController(RuntimeAnimatorController anim)
    {
        if(m_animator == null)
        {
            Debug.LogError("Player's Animator reference is null! Can't set the controller ");
            return;
        }
        m_animator.runtimeAnimatorController = anim;
    }
}
