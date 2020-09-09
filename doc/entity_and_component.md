# Entity 和 Component


## Unity的GameObject和Component结构

Scene - GameObject - Component

C#

## Component

UnityEngine.Object
        |
    Component
        |
    Behaviour
        |
   MonoBehaviour

UnityEngine.Object
{
    name
    GetInstanceID()
    GetHashCode()
    ToString()
}

Component
{
    gameObject, only get
    tag
    transform, only get
}

Behaviour
{
    enabled
}

MonoBehaviour
{
    没有字段和方法，但是有规定生命周期。Awake -> Start -> OnEnable -> Update -> OnDisable -> OnDestroy.
}


## GameObject

UnityEngine.Object
        |
    GameObject

GameObject
{
    activeSelf
    SetActive()
    AddComponent()
    GetComponent()
    GetComponentsInChildren()
}


## Scene

Scene
{
    rootCount
    GetRootGameObjects()
}

Unity的Scene类简单，没有继承树。


## 借鉴

cpp

World
{
    rootCount
    GetRootEntities()
    CreateEntity()  () -> entity
    AttachEntity()  (entity) ->
    GetComponentsInEnities()  <type> -> components_list
}

WorldObject
{
    name -> string
    GetInstanceID()  () -> int
    GetHashCode()  () -> int
    ToString()
}

Entity
{
    Actived()
    SetActive()
    OnwerWorld()
    AddComponent()  <type> -> component
    GetComponent()  <type> -> component
    GetComponentsInChildren()  <type> -> component_list
}

Component
{
    OnwerEntity()  () -> entity
    Enabled()  (status) -> ()
    Enable()  status -> ()
}