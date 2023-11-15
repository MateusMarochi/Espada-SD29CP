using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cube : MonoBehaviour
{
    public GameObject cubeObject;
    private Queue<Quaternion> rotationsQueue = new Queue<Quaternion>();

    void Update()
    {
        if (rotationsQueue.Count > 0)
        {
            Quaternion rotation = rotationsQueue.Dequeue();
            if (cubeObject != null)
            {
                cubeObject.transform.rotation = rotation;
            }
        }
    }

    public void SetRotation(Quaternion rotation)
    {
        rotationsQueue.Enqueue(rotation);
    }
}

