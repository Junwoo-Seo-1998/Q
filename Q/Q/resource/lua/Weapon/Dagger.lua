WeaponType="Melee"
IsAttacking=false
time = 0
MeleeDamage=100
function OnInit()
    print("dagger!")
end

function OnUpdate(dt)
	time = time - dt;
    if(Input.IsKeyPressed(Key.X)) then
	if(time < 0) then
        IsAttacking=true
        print("attack!")
        Sound_Component = Utils.GetSoundComponent()
        Sound_Component:PlaySoundEffect("sword-swing1",false)
        TestTraceBack.LogWeaponUseMessage("Dagger")
		time = 0.3
	end
		else 
        IsAttacking=false
	end

    
end

function OnCleanUp()

end