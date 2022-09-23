test=200

function test_func()

    return "This is Test!"
end 

function OnInit()
    print("init!")
end

function OnUpdate(dt)
    a=Vector2.new(1,2)
    b=Vector2.new(3,4)
    a=a+b
    a=a*a
    t=a==b
    print("hello!", a.x,a.y,t)

    if(Input.IsKeyPressed(Key.A)) then
       print("I AM!!", MyID)--print current entt id 
        --trans=State:GetTransformComponent(MyID)
        --pos=trans:GetPosition()
        --pos.x=pos.x+10
        --print("pos",pos.x) 
    end 
end

function OnCleanUp()
end