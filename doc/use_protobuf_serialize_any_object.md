# use_protobuf_serialize_any_object


## 万能的 key - value 序列化模版

WorldObject
{
    id : int
    name : string
    type : string
    members : [] -> member
    {
        key: string
        value: int
    }
}

bool_pool, x
int_pool, x
float_pool, v
double_pool, v
string_pool, v
bytes_pool, v


## 配套的接口实现

get_member()  (object, key) -> value
set_member()  (object, key, value) -> 

衍生
get_list_member()
set_list_member()

add_member()
add_element()


## python 端的封装

WorldObject
Entity
Bounds


Entity
{
    add_component()  (self, component_id) -> ()
}