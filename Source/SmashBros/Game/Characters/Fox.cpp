
#include "Fox.h"
#include "../../AttackTemplates.h"
#include "../../Global.h"
#include "../../P2PDataManager.h"
#include <cmath>

namespace SmashBros
{
	void Fox::addProjectileInfo(byte type, int projID, float x, float y)
	{
		ProjectileInfo info;
		info.type = type;
		info.projID = projID;
		info.x = x;
		info.y = y;
		createdProjectiles.add(info);
	}
	
	Fox::Fox(float x1, float y1, byte playerNo, byte team) : Player(x1, y1, playerNo, team)
	{
		finalSmashing = false;
		finalsmashFinishing = false;
		prevJumping = false;
		finalsmashTime = 0;
		lastKickFrame = 0;
		lastBUpChargeDir = 0;
		lastDir = 0;
		landmaster = null;
		
		walkSpeed = 4.5f;
		runSpeed = 7;
		fallWalk = 2.8f;
		fallRun = 3;
		
		weight = 0.29;
		
		name = "Fox";
		
		setHitbox(-10, -14, 20, 35);
		setHitboxColor(Color::GREEN);
		//showHitboxWireframe(true);
		//setWireframeColor(Color.red);
		//showWireframe(true);
		
		setHangPoint(15, 8);
		
		oldScale = Scale;
		
		Console::WriteLine((String)"finished creating player " + playerNo);
	}
	
	Fox::~Fox()
	{
		if(landmaster!=null)
		{
			landmaster = null;
		}
	}
	
	void Fox::addP2PData(DataVoid&data)
	{
		int total = createdProjectiles.size();
		
		if(total>0)
		{
			P2PDataManager::setReliable();
			
			bool avail = true;
			data.add(&avail, sizeof(avail));
			
			int total = createdProjectiles.size();
			data.add(&total, sizeof(total));
			
			for(int i=0; i<createdProjectiles.size(); i++)
			{
				ProjectileInfo proj = createdProjectiles.get(i);
				
				data.add(&proj.type, sizeof(proj.type));
				data.add(&proj.projID, sizeof(proj.projID));
				data.add(&proj.x, sizeof(proj.x));
				data.add(&proj.y, sizeof(proj.y));
			}
			createdProjectiles.clear();
		}
		else
		{
			bool avail = false;
			data.add(&avail, sizeof(avail));
		}
	}
	
	void Fox::setP2PData(byte*&data)
	{
		bool avail = data[0];
		data += sizeof(bool);
		
		if(avail)
		{
			int total = DataVoid::toInt(data);
			data += sizeof(int);
			
			for(int i=0; i<total; i++)
			{
				byte type = data[0];
				data += sizeof(byte);
				
				int projID = DataVoid::toInt(data);
				data += sizeof(int);
				
				float x1 = DataVoid::toFloat(data);
				data += sizeof(float);
				float y1 = DataVoid::toFloat(data);
				data += sizeof(float);
				
				switch(type)
				{
					case 1:
					{
						Projectile::setNextID(projID);
						createProjectile(new Ray(getPlayerNo(), x1, y1));
					}
					break;
					
					case 2:
					{
						Projectile::setNextID(projID);
						createProjectile(new LandmasterShot(getPlayerNo(), x1, y1));
					}
					break;
					
					case 3:
					{
						Projectile::setNextID(projID);
						landmaster = new Landmaster(getPlayerNo(), x1, y1);
						createProjectile(landmaster);
					}
					break;
				}
			}
		}
	}
	
	void Fox::Load()
	{
		setFolderPath("Images/Game/Characters/Fox/");
		
		loadFile("Images/Game/Characters/Fox/ray.png");
		loadFile("Images/Game/Characters/Fox/landmaster_idle.png");
		loadFile("Images/Game/Characters/Fox/landmaster_move.png");
		loadFile("Images/Game/Characters/Fox/landmaster_hover.png");
		loadFile("Images/Game/Characters/Fox/landmaster_shoot.png");
		loadFile("Images/Game/Characters/Fox/landmaster_boost.png");
		loadFile("Images/Game/Characters/Fox/landmaster_shot.png");
		
		addTwoSidedAnimation("stand", "stand.png", 4, 6, 1);
		addTwoSidedAnimation("walk", "walk.png", 6, 10, 1);
		addTwoSidedAnimation("run", "run.png", 12, 10, 1);
		addTwoSidedAnimation("jump", "jump.png", 5, 3, 1);
		addTwoSidedAnimation("jump2", "jump2.png", 20, 7, 1);
		addTwoSidedAnimation("land", "land.png", 9, 3, 1);
		addTwoSidedAnimation("fall", "fall.png", 10, 2, 1);
		addTwoSidedAnimation("hang", "hang.png", 1, 1, 1);
		addTwoSidedAnimation("crouch", "crouch.png", 1, 1, 1);
		addTwoSidedAnimation("melee_weapon", "melee_weapon.png", 8, 4, 1);
		addTwoSidedAnimation("hurt_minor", "hurt_minor.png", 30, 1, 1);
		addTwoSidedAnimation("hurt_fly", "hurt_fly.png", 30, 1, 1);
		addTwoSidedAnimation("hurt_flip", "hurt_flip.png", 18, 7, 1);
		addTwoSidedAnimation("hurt_spin", "hurt_spin.png", 10, 4, 1);
		addSingleAnimation("hurt_spin_up", "hurt_spin_up.png", 10, 4, 1);
		//addTwoSidedAnimation("dash_attack", "dash_attack.png", 12, 6, 1);
		addTwoSidedAnimation("standard_attack", "standard_attack.png", 14, 4, 1);
		addTwoSidedAnimation("standard_attack2", "standard_attack2.png", 14, 4, 1);
		addTwoSidedAnimation("standard_attack3", "standard_attack3.png", 10, 6, 1);
		addTwoSidedAnimation("standard_attack_side", "standard_attack_side.png", 16, 4, 1);
		addTwoSidedAnimation("standard_attack_up", "standard_attack_up.png", 21, 7, 1);
		addTwoSidedAnimation("standard_attack_down", "standard_attack_down.png", 10, 4, 1);
		addTwoSidedAnimation("air_attack", "air_attack.png", 8, 2, 1);
		addTwoSidedAnimation("air_attack_side", "air_attack_side.png", 18, 6, 1);
		addTwoSidedAnimation("air_attack_up", "air_attack_up.png", 10, 5, 1);
		addTwoSidedAnimation("air_attack_down", "air_attack_down.png", 18, 8, 1);
		addTwoSidedAnimation("special_attack", "special_attack.png", 12, 4, 1);
		addTwoSidedAnimation("special_prep_side", "special_prep_side.png", 12, 3, 1);
		addTwoSidedAnimation("special_attack_side", "special_attack_side.png", 50, 7, 1);
		addTwoSidedAnimation("special_finish_side", "special_finish_side.png", 18, 3, 1);
		addTwoSidedAnimation("special_charge_up", "special_charge_up.png", 30, 21, 1);
		addTwoSidedAnimation("special_attack_up_up", "special_attack_up_up.png", 24, 12, 1);
		addTwoSidedAnimation("special_attack_up_upside", "special_attack_up_upside.png", 24, 12, 1);
		addTwoSidedAnimation("special_attack_up_side", "special_attack_up_side.png", 24, 12, 1);
		addTwoSidedAnimation("special_attack_up_downside", "special_attack_up_downside.png", 24, 12, 1);
		addTwoSidedAnimation("special_attack_up_down", "special_attack_up_down.png", 24, 12, 1);
		addTwoSidedAnimation("special_attack_down", "special_attack_down.png", 40, 11, 1);
		addTwoSidedAnimation("smash_charge", "smash_charge.png", 1, 1, 1);
		addTwoSidedAnimation("smash_attack", "smash_attack.png", 16, 7, 1);
		addTwoSidedAnimation("smash_attack_up", "smash_attack_up.png", 14, 8, 1);
		addTwoSidedAnimation("smash_attack_down", "smash_attack_down.png", 16, 10, 1);
	}
	
	void Fox::LoadAttackTypes()
	{
		addAIAttackType(ATTACK_A,         ATTACKTYPE_MELEE, 4);
		addAIAttackType(ATTACK_SIDEA,     ATTACKTYPE_MELEE, 3);
		addAIAttackType(ATTACK_UPA,       ATTACKTYPE_MELEE, 1);
		addAIAttackType(ATTACK_UPA,       ATTACKTYPE_UPMELEE, 2);
		addAIAttackType(ATTACK_DOWNA,     ATTACKTYPE_MELEE, 2);
		addAIAttackType(ATTACK_DOWNA,     ATTACKTYPE_DOWNMELEE, 2);
		addAIAttackType(ATTACK_B,         ATTACKTYPE_PROJECTILE, 2);
		addAIAttackType(ATTACK_SIDEB,     ATTACKTYPE_SIDEMOVE, 3);
		addAIAttackType(ATTACK_UPB,       ATTACKTYPE_UPMOVE, 1);
		addAIAttackType(ATTACK_UPB,       ATTACKTYPE_SIDEMOVE, 1);
		addAIAttackType(ATTACK_DOWNB,     ATTACKTYPE_DEFENSIVE, 1);
		addAIAttackType(ATTACK_SIDESMASH, ATTACKTYPE_STRONGMELEE, 2, true);
		addAIAttackType(ATTACK_SIDESMASH, ATTACKTYPE_SIDEMOVE, 2, true);
		addAIAttackType(ATTACK_UPSMASH,   ATTACKTYPE_STRONGMELEE, 1, true);
		addAIAttackType(ATTACK_DOWNSMASH, ATTACKTYPE_STRONGMELEE, 1, true);
		addAIAttackType(ATTACK_FINALSMASH,ATTACKTYPE_PROJECTILE, 1, true);
	}
	
	void Fox::onDestroy()
	{
		if(finalSmashing)
		{
			finalSmashing = false;
			finalsmashFinishing = false;
			Scale = oldScale;
			setVisible(true);
		}
		if(landmaster!=null)
		{
			landmaster->destroy();
			landmaster = null;
		}
	}
	
	void Fox::onAnimationFinish(const String&n)
	{
		if(n.equals("special_prep_side_left") || n.equals("special_prep_side_right"))
		{
			landing=false;
	        yvelocity=0;
	        if(finalSmashing)
	        {
	        	animFinish();
	        }
	        else
	        {
	        	AttackTemplates::dashSideB(this, 12, 4.24, 16);
	        }
		}
		else if(n.equals("special_attack_side_left") || n.equals("special_attack_side_right"))
		{
			animFinish();
	        xVel=0;
	        landing=true;
	        yvelocity=0;
	        switch(playerdir)
	        {
	            case 1:
	            changeAnimation("special_finish_side_left", FORWARD);
	            break;


	            case 2:
	            changeAnimation("special_finish_side_right", FORWARD);
	            break;
	        }
		}
		else if(n.equals("special_charge_up_left") || n.equals("special_charge_up_right"))
		{
			xvelocity = 0;
			yvelocity = 0;
			
			attacksHolder = 14;
			attacksPriority = 4.56;
			
			bUp = true;
			
			byte currentDir = 0;
			
			if(lastBUpChargeDir!=0)
			{
				currentDir = lastBUpChargeDir;
			}
			else
			{
				currentDir = getDirFacing();
			}
			
			switch(currentDir)
			{
				default:
				case DIR_UP:
				switch(playerdir)
				{
					case LEFT:
					changeAnimation("special_attack_up_up_left", FORWARD);
					break;
					
					case RIGHT:
					changeAnimation("special_attack_up_up_right", FORWARD);
					break;
				}
				yVel = -5;
				break;
				
				case DIR_DOWN:
				switch(playerdir)
				{
					case LEFT:
					changeAnimation("special_attack_up_down_left", FORWARD);
					break;
					
					case RIGHT:
					changeAnimation("special_attack_up_down_right", FORWARD);
					break;
				}
				yVel = 5;
				break;
				
				case DIR_LEFT:
				changeAnimation("special_attack_up_side_left", FORWARD);
				xVel = -5;
				break;
				
				case DIR_RIGHT:
				changeAnimation("special_attack_up_side_right", FORWARD);
				xVel = 5;
				break;
				
				case DIR_UPLEFT:
				changeAnimation("special_attack_up_upside_left", FORWARD);
				yVel = -3.5f;
				xVel = -3.5f;
				break;
				
				case DIR_UPRIGHT:
				changeAnimation("special_attack_up_upside_right", FORWARD);
				yVel = -3.5f;
				xVel = 3.5f;
				break;
				
				case DIR_DOWNLEFT:
				changeAnimation("special_attack_up_downside_left", FORWARD);
				yVel = 3.5f;
				xVel = -3.5f;
				break;
				
				case DIR_DOWNRIGHT:
				changeAnimation("special_attack_up_downside_right", FORWARD);
				yVel = 3.5f;
				xVel = 3.5f;
				break;
			}
			
			lastBUpChargeDir = 0;
			lastDir = 0;
		}
		else
		{
			Player::onAnimationFinish(n);
		}
	}
	
	void Fox::jump()
	{
		Player::jump(7,(float)7.2);
	}
	
	void Fox::Update(long gameTime)
	{
		if(attacksHolder==2)
		{
			Animation*anim = getLastAnimation();
			if((anim->getCurrentFrame()==1 || anim->getCurrentFrame()==3 || anim->getCurrentFrame()==5)
				&& lastKickFrame!=anim->getCurrentFrame())
			{
				resetAttackCollisions();
			}
			lastKickFrame = anim->getCurrentFrame();
		}
		
		if(attacksHolder==13 || attacksHolder==14)
		{
			xvelocity = 0;
			yvelocity = 0;
		}
		
		Player::Update(gameTime);
		checkAttacks();
		
		if(attacksHolder==13)
		{
			byte newDir = getDirFacing();
			if(newDir!=0)
			{
				if(!((lastDir==DIR_UPLEFT && (newDir==DIR_UP || newDir==DIR_LEFT))
				   ||(lastDir==DIR_UPRIGHT && (newDir==DIR_UP || newDir==DIR_RIGHT))
				   ||(lastDir==DIR_DOWNLEFT && (newDir==DIR_DOWN || newDir==DIR_LEFT))
				   ||(lastDir==DIR_DOWNRIGHT && (newDir==DIR_DOWN || newDir==DIR_RIGHT))))
				{
					lastBUpChargeDir = newDir;
				}
			}
			lastDir = newDir;
		}
		else
		{
			updateGravity();
		}
		updateFrame();
		
		if(attacksHolder==19)
		{
		    setAlpha(getAlpha()+0.05f);
		    if(getAlpha()>=1)
		    {
		    	animFinish();
	            attacksHolder=-1;
	            attacksPriority=7;
	            setAlpha(0);
	            setVisible(false);
	            Scale = 0;
	            finalSmashing=true;
	            finalsmashFinishing=false;
	            finalsmashTime = Global::getWorldTime() + 180;
	            float y1 = y - 200;
				if(y1<((float)Global::currentStage->getItemBoundaries().y))
				{
					y1=(float)Global::currentStage->getItemBoundaries().y;
				}
	            landmaster = new Landmaster(getPlayerNo(),x,y1);
	            createProjectile(landmaster);
				
				addProjectileInfo(3, landmaster->getID(), x, y1);
	            
		        animFinish();
		    }
		}
		else if(finalSmashing)
		{
			if(finalsmashTime<=Global::getWorldTime() || finalsmashFinishing)
			{
				finalsmashFinishing = true;
				x = landmaster->x;
				y = landmaster->y;
				yvelocity = 0;
				xvelocity = 0;
				yVel = 0;
				xVel = 0;
				attacksHolder = -1;
				attacksPriority = 7;
				doubleJump = 1;
				bUp = false;
				landmaster->setAlpha(landmaster->getAlpha()+0.05f);
				if(landmaster->getAlpha()>=1)
				{
					landmaster->destroy();
					landmaster = null;
					AttackTemplates::finishFinalSmash(this);
					finalSmashing = false;
					Scale = oldScale;
					setVisible(true);
					float y1 = y - 200;
					if(y1<((float)Global::currentStage->getItemBoundaries().y))
					{
						y1=(float)Global::currentStage->getItemBoundaries().y;
					}
					y = y1;
				}
			}
			else
			{
				if(landmaster->getAttackNo()==-1)
				{
					if((attacksHolder>=0 && attacksHolder<=10)
					||(attacksHolder>=16 && attacksHolder<=18))
					{
						landmaster->shoot();
					}
				}
				
				x = landmaster->x;
				y = landmaster->y;
				yvelocity = 0;
				xvelocity = 0;
				yVel = 0;
				xVel = 0;
				attacksHolder = -1;
				attacksPriority = 7;
				doubleJump = 1;
				bUp = false;
				
				if(isJumping() && prevJumping)
				{
					setJumping(false);
				}
				prevJumping = isJumping();
			}
		}
		
		if(!finalSmashing)
		{
			updateMovement();
		}
		
		updateHanging();
		updateHurt();
		updateDrop();
		updateAI();
	}
	
	void Fox::onPlayerHit(Player*collide, byte dir)
	{
		switch(dir)
		{
			case DIR_LEFT:
			switch(attacksHolder)
		    {
		        case 0:
		        //A1
		        if(playerdir==LEFT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=3;
			        causeHurt(collide, RIGHT, 1);
		        }
		        break;
		        
		        case 1:
		        //A2
		        if(playerdir==LEFT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=3;
			        causeHurt(collide, RIGHT, 1);
		        }
		        break;
		        
		        case 2:
		        //A3
		        if(playerdir==LEFT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=5;
			        collide->x-=3;
			        causeHurt(collide, RIGHT, 1);
		        }
		        break;
		        
		        case 3:
		        //A Dash
		        break;
		        
		        case 16:
		        //Smash Side
		        if(playerdir==LEFT)
		        {
			        causeDamage(collide, smashPower);
			        causeHurtLaunch(collide, -1,((float)smashPower/6),4.3f, -1,2,2);
			        causeHurt(collide, RIGHT, 6);
		        }
		        break;
		        
		        case 17:
		        //Smash Up
		        causeDamage(collide, smashPower);
		        causeHurtLaunch(collide, -1,2,2, -1,((float)smashPower/5),4.3f);
		        causeHurt(collide, RIGHT, 6);
		        break;
		        
		        case 18:
		        //Smash Down
		        causeDamage(collide, smashPower);
		        causeHurtLaunch(collide, -1,((float)smashPower/5),4.1f, -1,((float)smashPower/8),2.1f);
		        causeHurt(collide, RIGHT, 6);
		        break;
		    }
			break;
			
			case DIR_RIGHT:
			switch(attacksHolder)
		    {
		        case 0:
		        //A1
		        if(playerdir==RIGHT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=3;
			        causeHurt(collide, LEFT, 1);
		        }
		        break;
		        
		        case 1:
		        //A2
		        if(playerdir==RIGHT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=3;
			        causeHurt(collide, LEFT, 1);
		        }
		        break;
		        
		        case 2:
		        //A3
		        if(playerdir==RIGHT)
		        {
			        causeDamage(collide, 2);
			        collide->y-=5;
			        collide->x+=3;
			        causeHurt(collide, LEFT, 1);
		        }
		        break;
		        
		        case 3:
		        //A Dash
		        break;
		        
		        case 16:
		        //Smash Side
		        if(playerdir==RIGHT)
		        {
			        causeDamage(collide, smashPower);
			        causeHurtLaunch(collide, 1,((float)smashPower/6),4.3f, -1,2,2);
			        causeHurt(collide, LEFT, 6);
		        }
		        break;
		        
		        case 17:
		        //Smash Up
		        causeDamage(collide, smashPower);
		        causeHurtLaunch(collide, 1,2,2, -1,((float)smashPower/5),4.3f);
		        causeHurt(collide, LEFT, 6);
		        break;
		        
		        case 18:
		        //Smash Down
		        causeDamage(collide, smashPower);
		        causeHurtLaunch(collide, 1,((float)smashPower/5),4.1f, -1,((float)smashPower/8),2.1f);
		        causeHurt(collide, LEFT, 6);
		        break;
		    }
			break;
			
			case DIR_UP:
			switch(attacksHolder)
		    {
		        case 17:
		        //Smash Up
		        causeDamage(collide, smashPower);
		        switch(playerdir)
		        {
		        	case LEFT:
			        causeHurtLaunch(collide, -1,2,2, -1,((float)smashPower/5),4.3f);
			        causeHurt(collide, RIGHT, 6);
			        break;
			        
		        	case RIGHT:
		        	causeHurtLaunch(collide, 1,2,2, -1,((float)smashPower/5),4.3f);
		        	causeHurt(collide, LEFT, 6);
		        	break;
		        }
		        break;
		    }
			break;
			
			case DIR_DOWN:
			switch(attacksHolder)
		    {
		        case 18:
		        //Smash Down
		        causeDamage(collide, smashPower);
		        switch(playerdir)
		        {
		        	case LEFT:
		        	causeHurtLaunch(collide, -1,((float)smashPower/5),4.1f, 1,((float)smashPower/8),2.1f);
			        causeHurt(collide, RIGHT, 6);
			        break;
			        
		        	case RIGHT:
		        	causeHurtLaunch(collide, 1,((float)smashPower/5),4.1f, 1,((float)smashPower/8),2.1f);
		        	causeHurt(collide, LEFT, 6);
		        	break;
		        }
		        break;
		    }
			break;
		}
		
		if(attacksHolder==13)
	    {
	    	causeDamage(collide,2);
	        collide->y=y-60;
	        if(collide->x<x)
	        {
	            collide->x+=2;
	            causeHurt(collide, RIGHT, 1);
	        }
	        else
	        {
	            collide->x-=2;
	            causeHurt(collide, LEFT, 1);
	        }
	        collide->xvelocity=0;
	    }
		else if(attacksHolder==14)
		{
			double dist = distance(x,y,collide->x,collide->y);
			float xDif = collide->x - x;
			float yDif = collide->y - y;
			
			int multX = 1;
			int multY = 1;
			
			causeDamage(collide, 8);
			
			if(collide->x<x)
			{
				multX = -1;
			}
			if(collide->y<y)
			{
				multY = -1;
			}
			
			causeHurtLaunch(collide, multX,(float)std::abs(xDif/dist)*3,2.6f, multY,(float)std::abs(yDif/dist)*3,2.6f);
			
			if(collide->x<x)
			{
				causeHurt(collide, RIGHT, 6);
			}
			else
			{
				causeHurt(collide, LEFT, 6);
			}
		}
	}
	
	void Fox::onFinishCharge()
	{
		//
	}
	
	void Fox::doChargingAttack(byte button)
	{
		//
	}
	
	void Fox::attackA()
	{
		addAttackInfo(DIR_LEFT, 7, LEFT, 7, 4, -1,3.2f,4, -1,1,4);
		addAttackInfo(DIR_RIGHT,7,RIGHT, 7, 4,  1,3.2f,4, -1,1,4);
		
	    if(!checkItemUse())
	    {
	        if(isOnGround())
	        {
	            switch(playerdir)
	            {
	                case LEFT:
	                x-=5;
	                break;
	 
	                case 2:
	                x+=5;
	                break;
	            }
	            AttackTemplates::combo3A(this, 5, 0,1.98, 1,2.21, 2,3.04);
	        }
	        else
	        {
	            if(!bUp)
	            {
	                AttackTemplates::normalAirA(this, 7,1.789);
	            }
	        }
	    }
	}
	
	void Fox::attackSideA()
	{
		addAttackInfo(DIR_LEFT, 4, LEFT, 6, 4, -1,2,2, -1,3,3.5f);
		addAttackInfo(DIR_RIGHT,4,RIGHT, 6, 4,  1,2,2, -1,3,3.5f);
		addAttackInfo(DIR_LEFT, 8, LEFT, 8, 4, -1,4,3.2f, -1,1,2);
		addAttackInfo(DIR_RIGHT,8,RIGHT, 8, 4,  1,4,3.2f, -1,1,2);
		addAttackInfo(DIR_UP, 8, LEFT, 8, 4, -1,4,3.2f, -1,1,2);
		addAttackInfo(DIR_UP, 8,RIGHT, 8, 4,  1,4,3.2f, -1,1,2);
		
	    //if(((playr->moveLeft==2) || (playr->moveRight==2)) && (playr->onGround==1))
	    //{
	        //normalDashA(playr, 3,2.3, 3.5);
	    //}
	    //else
	    //{
	    	if(!checkItemUseSide())
	        {
	            if(isOnGround())
	            {
	                switch(playerdir)
	                {
	                    case LEFT:
	                    x-=5;
	                    break;
	 
	                    case RIGHT:
	                    x+=5;
	                    break;
	                }
	                AttackTemplates::normalSideA(this, 4,2.411);
	            }
	            else
	            {
	                if(!bUp)
	                {
	                    AttackTemplates::normalAirSideA(this, 8,2.234);
	                    switch(playerdir)
	                    {
	                        case LEFT:
	                        xvelocity-=1;
	                        break;
	 
	                        case RIGHT:
	                        xvelocity+=1;
	                        break;
	                    }
	                    yvelocity-=1.5;
	                }
	            }
	        }
	    //}
	}
	
	void Fox::attackUpA()
	{
		addAttackInfo(DIR_LEFT, 5, LEFT, 3, 4, -1,3,2.7f, -1,2,2);
		addAttackInfo(DIR_LEFT, 5,RIGHT, 3, 4, -1,3,2.7f, -1,2,2);
		addAttackInfo(DIR_RIGHT, 5, LEFT, 3, 4, 1,3,2.7f, -1,2,2);
		addAttackInfo(DIR_RIGHT, 5,RIGHT, 3, 4, 1,3,2.7f, -1,2,2);
		addAttackInfo(DIR_LEFT, 9, LEFT, 9, 5, -1,0.9f,1.2f, -1,2.1f,4.1f);
		addAttackInfo(DIR_LEFT, 9,RIGHT, 9, 5, -1,0.9f,1.2f, -1,2.1f,4.1f);
		addAttackInfo(DIR_RIGHT,9, LEFT, 9, 5,  1,0.9f,1.2f, -1,2.1f,4.1f);
		addAttackInfo(DIR_RIGHT,9,RIGHT, 9, 5,  1,0.9f,1.2f, -1,2.1f,4.1f);
		addAttackInfo(DIR_UP, 5, LEFT, 3, 4, -1,0.5f,1.1f, -1,3,2.8f);
		addAttackInfo(DIR_UP, 5,RIGHT, 3, 4,  1,0.5f,1.1f, -1,3,2.8f);
		addAttackInfo(DIR_UP, 9, LEFT, 9, 5, -1,0.9f,1.2f, -1,2.1f,4.1f);
		addAttackInfo(DIR_UP, 9,RIGHT, 9, 5,  1,0.9f,1.2f, -1,2.1f,4.1f);
		
		if(isOnGround())
	    {
	        AttackTemplates::normalUpA(this, 5, 2.911);
	    }
		else
		{
	        if(!bUp)
	        {
	            AttackTemplates::normalAirUpA(this, 9,2.956);
	        }
	    }
	}
	
	void Fox::attackDownA()
	{
		addAttackInfo(DIR_LEFT, 6, LEFT, 9, 4, -1,3,2, -1,3,2);
		addAttackInfo(DIR_RIGHT,6,RIGHT, 9, 4,  1,3,2, -1,3,2);
		addAttackInfo(DIR_LEFT, 10, LEFT, 7, 4, -1,0.9f,1.2f, 1,2.1f,4.1f);
		addAttackInfo(DIR_RIGHT,10,RIGHT, 7, 4,  1,0.9f,1.2f, 1,2.1f,4.1f);
		addAttackInfo(DIR_DOWN, 6, LEFT, 9, 4, -1,2,1.4f, 1,3.5f,3.4f);
		addAttackInfo(DIR_DOWN, 6,RIGHT, 9, 4,  1,2,1.4f, 1,3.5f,3.4f);
		addAttackInfo(DIR_DOWN, 10, LEFT, 7, 4, -1,0.9f,1.2f, 1,3.1f,4.1f);
		addAttackInfo(DIR_DOWN, 10,RIGHT, 7, 4,  1,0.9f,1.2f, 1,3.1f,4.1f);
		
		if(isOnGround())
	    {
	        switch(playerdir)
	        {
	            case LEFT:
	            x-=3;
	            break;
	 
	            case RIGHT:
	            x+=3;
	            break;
	        }
	        AttackTemplates::normalDownA(this, 6,1.399);
	    }
		else
		{
	        if(!bUp)
	        {
	            AttackTemplates::normalAirDownA(this, 10,2.454);
	        }
	    }
	}
	
	void Fox::attackB()
	{
		if(!bUp)
	    {
	        if(canFinalSmash())
	        {
	            attackFinalSmash();
	        }
	        else
	        {
	            int x1=25;
	            switch(playerdir)
	            {
	                case LEFT:
	                x1=-25;
	                changeAnimation("special_attack_left",FORWARD);
	                break;
	 
	                case RIGHT:
	                x1=25;
	                changeAnimation("special_attack_right",FORWARD);
	                break;
	            }
	            if(!finalSmashing)
	            {
					Ray*ray = new Ray(getPlayerNo(),x + x1, y);
	            	AttackTemplates::singleProjectile(this, 11, 0, ray);
					
					addProjectileInfo(1, ray->getID(), x + x1, y);
	            }
	        }
	    }
	}
	
	void Fox::attackSideB()
	{
		addAttackInfo(DIR_LEFT, 12, LEFT, 7, 5, -1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_LEFT, 12,RIGHT, 7, 5, -1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_RIGHT, 12, LEFT, 7, 5, 1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_RIGHT, 12,RIGHT, 7, 5, 1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_UP, 12, LEFT, 7, 5, -1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_UP, 12,RIGHT, 7, 5,  1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_DOWN, 12, LEFT, 7, 5, -1,0.5f,1.2f, -1,4,1.6f);
		addAttackInfo(DIR_DOWN, 12,RIGHT, 7, 5,  1,0.5f,1.2f, -1,4,1.6f);
		
        if(!bUp)
        {
            bUp=true;
            landing=true;
            switch(playerdir)
            {
                case 1:
                changeAnimation("special_prep_side_left", FORWARD);
                break;

                case 2:
                changeAnimation("special_prep_side_right", FORWARD);
                break;
            }
        }
	}
	
	void Fox::attackUpB()
	{
		if(!bUp)
        {
            bUp=true;
            attacksPriority=4.1;
            attacksHolder=13;
            xvelocity = 0;
            yvelocity = 0;
            switch(playerdir)
            {
                case LEFT:
                changeAnimation("special_charge_up_left", FORWARD);
                break;


                case 2:
                changeAnimation("special_charge_up_right", FORWARD);
                break;
            }
            
            lastBUpChargeDir = 0;
			lastDir = 0;
        }
	}
	
	void Fox::attackDownB()
	{
	    if(!bUp)
	    {
	        AttackTemplates::normalDownB(this, 15,6);
	    }
	}
	
	void Fox::attackSideSmash(byte type)
	{
		if(!checkItemUseSmash(type)&&!bUp)
	    {
	        if(isOnGround())
	        {
	            AttackTemplates::normalSmash(this,16,3.557,type,1,30,15,42);
	            if(type==STEP_GO)
	            {
	                yvelocity=-2*(((float)smashPower)/18);
	                switch(playerdir)
	                {
	                    case 1:
	                    xvelocity=-3*(((float)smashPower)/18);
	                    break;
	 
	                    case 2:
	                    xvelocity=3*(((float)smashPower)/18);
	                    break;
	                }
	            }
	        }
	        else
	        {
	            attackSideA();
	        }
	    }
	}
	
	void Fox::attackUpSmash(byte type)
	{
		if(!bUp)
	    {
	        if(isOnGround())
	        {
	            AttackTemplates::normalSmash(this,17,3.224,type,2,30,17,31);
	        }
	        else
	        {
	            attackUpA();
	        }
	    }
	}
	
	void Fox::attackDownSmash(byte type)
	{
		if(!bUp)
	    {
	        if(isOnGround())
	        {
	            AttackTemplates::normalSmash(this,18,3.043,type,3,20,14,30);
	        }
	        else
	        {
	            attackDownA();
	        }
	    }
	}
	
	void Fox::attackFinalSmash()
	{
	    if(!bUp)
	    {
            landing=true;
            destroyCharge();
            AttackTemplates::finalSmash(this, 19);
            setAlpha(0);
            discardItem();
            switch(playerdir)
            {
                case 1:
                changeAnimation("crouch_left", FORWARD);
                break;
 
                case 2:
                changeAnimation("crouch_right", FORWARD);
                break;
            }
	    }
	}
	
	Fox::Ray::Ray(byte playerNo, float x1, float y1) : Projectile(playerNo, x1, y1)
	{
		addAnimation(new Animation("normal",1,"Images/Game/Characters/Fox/ray.png"));
		changeAnimation("normal",FORWARD);
			
		switch(itemdir)
		{
			case LEFT:
			xvelocity=-7.5f;
			break;
				
			case RIGHT:
			xvelocity=7.5f;
			break;
		}
	}

	Fox::Ray::~Ray()
	{
		//
	}
	
	void Fox::Ray::onPlayerHit(Player*collide, byte dir)
	{
		if(isHittable(collide,dir))
		{
			causeDamage(collide, 1);
			destroy();
		}
	}
	
	Fox::LandmasterShot::LandmasterShot(byte playerNo, float x1, float y1) : Projectile(playerNo, x1,y1)
	{
		Animation*anim;
		
		anim = new Animation("left",12,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_shot.png");
		addAnimation(anim);
		
		anim = new Animation("right",12,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_shot.png");
		anim->mirror(true);
		addAnimation(anim);
			
		switch(itemdir)
		{
			case LEFT:
			changeAnimation("left",FORWARD);
			xvelocity = -5;
			break;
				
			case RIGHT:
			changeAnimation("right",FORWARD);
			xvelocity = 5;
			break;
		}
			
		setLayer(LAYER_MIDDLEUPPER);
	}
	
	Fox::LandmasterShot::~LandmasterShot()
	{
		//
	}

	void Fox::LandmasterShot::onPlayerHit(Player*collide, byte dir)
	{
		if(isHittable(collide,dir))
		{
			causeDamage(collide,17);
			if(collide->x<x)
			{
			    causeHurtLaunch(collide, -1,6,6, -1,3,6);
			    causeHurt(collide, RIGHT, 4);
			}
			else if(x<collide->x)
			{
			    causeHurtLaunch(collide, 1,6,6, -1,3,6);
			    causeHurt(collide, LEFT, 4);
			}
			else
			{
			    causeHurtLaunch(collide, 1,0,0, -1,6,6);
			    causeHurt(collide, collide->getPlayerDir(), 4);
			}
		}
	}
	
	Fox::Landmaster::Landmaster(byte playerNo, float x1, float y1) : Projectile(playerNo, x1, y1)
	{
		Animation*anim;
		
		anim = new Animation("idle_left",1,"Images/Game/Characters/Fox/landmaster_idle.png");
		addAnimation(anim);
		anim = new Animation("idle_right",1,"Images/Game/Characters/Fox/landmaster_idle.png");
		anim->mirror(true);
		addAnimation(anim);
		
		anim = new Animation("move_left",8,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_move.png");
		addAnimation(anim);
		anim = new Animation("move_right",8,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_move.png");
		anim->mirror(true);
		addAnimation(anim);
		
		anim = new Animation("hover_left",10,3,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_hover.png");
		addAnimation(anim);
		anim = new Animation("hover_right",10,3,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_hover.png");
		anim->mirror(true);
		addAnimation(anim);
		
		anim = new Animation("shoot_left",10,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_shoot.png");
		addAnimation(anim);
		anim = new Animation("shoot_right",10,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_shoot.png");
		anim->mirror(true);
		addAnimation(anim);
			
		anim = new Animation("boost_left",8,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_boost.png");
		addAnimation(anim);
		anim = new Animation("boost_right",8,4,1);
		anim->addFrame("Images/Game/Characters/Fox/landmaster_boost.png");
		anim->mirror(true);
		addAnimation(anim);
			
		Scale = 0.8f;
			
		setSolid(true);
		setOwnerSolid(false);
			
		weight = 0.1f;
			
		attack = -1;
			
		idle();
	}

	Fox::Landmaster::~Landmaster()
	{
		Fox*fox = (Fox*)Global::getPlayerActor(getPlayerNo());
		if(fox!=NULL)
		{
			fox->landmaster = NULL;
		}
	}
		
	void Fox::Landmaster::onAnimationFinish(const String&n)
	{
		if(n.equals("hover_left") || n.equals("hover_right"))
		{
			hovering = false;
			idle();
		}
		else if(n.equals("shoot_left") || n.equals("shoot_right"))
		{
			Player*owner = Global::getPlayerActor(getPlayerNo());
			Fox*fox = (Fox*)owner;
			switch(attack)
			{
				case 0:
				{
					owner->setPlayerDir(LEFT);
					LandmasterShot*shot = new LandmasterShot(getPlayerNo(),x - (120*Scale), y - (24*Scale));
					createProjectile(shot);
					
					fox->addProjectileInfo(2, shot->getID(), x - (120*Scale), y - (24*Scale));
				}
				break;
					
				case 1:
				{
					owner->setPlayerDir(RIGHT);
					LandmasterShot*shot = new LandmasterShot(getPlayerNo(),x + (120*Scale), y - (24*Scale));
					createProjectile(shot);
					
					fox->addProjectileInfo(2, shot->getID(), x + (120*Scale), y - (24*Scale));
				}
				break;
			}
			idle();
		}
	}
		
	void Fox::Landmaster::Update(long gameTime)
	{
		Projectile::Update(gameTime);
		yvelocity+=(float)(Global::currentStage->getGravity() + weight);
			
		Player*owner = Global::getPlayerActor(getPlayerNo());
			
		if(attack==-1)
		{
			if(owner->isJumping() && !hovering)
			{
				hovering = true;
				switch(owner->getPlayerDir())
				{
					case LEFT:
					changeAnimation("hover_left",FORWARD);
					break;
						
					case RIGHT:
					changeAnimation("hover_right",FORWARD);
					break;
				}
				yvelocity -=5;
			}
			if(owner->getMoveLeft()>0)
			{
				x-=1;
				owner->setPlayerDir(LEFT);
				if(hovering)
				{
					changeAnimation("hover_left",NO_CHANGE);
				}
				else
				{
					changeAnimation("move_left",NO_CHANGE);
				}
			}
			else if(owner->getMoveRight()>0)
			{
				x+=1;
				owner->setPlayerDir(RIGHT);
				if(hovering)
				{
					changeAnimation("hover_right",NO_CHANGE);
				}
				else
				{
					changeAnimation("move_right",NO_CHANGE);
				}
			}
			else
			{
				if(!hovering)
				{
					switch(owner->getPlayerDir())
					{
						case LEFT:
						changeAnimation("idle_left",FORWARD);
						break;
							
						case RIGHT:
						changeAnimation("idle_right",FORWARD);
						break;
					}
				}
			}
		}
	}
		
	int Fox::Landmaster::getAttackNo()
	{
		return attack;
	}
	
	void Fox::Landmaster::idle()
	{
		attack=-1;
		//playr->attacksPriority=0;
		Fox*owner = (Fox*)Global::getPlayerActor(getPlayerNo());
		owner->animFinish();
		owner->attacksHolder=-1;
		owner->attacksPriority=7;
		owner->animFinish();
		owner->bUp=false;
		switch(owner->playerdir)
		{
		    case 1:
		    changeAnimation("idle_left", FORWARD);
		    break;
		 
		    case 2:
		    changeAnimation("idle_right", FORWARD);
		    break;
		}
	}
		
	void Fox::Landmaster::shoot()
	{
		Player*owner = Global::getPlayerActor(getPlayerNo());
		switch(owner->getPlayerDir())
		{
			case LEFT:
			changeAnimation("shoot_left",FORWARD);
			attack = 0;
			break;
				
			case RIGHT:
			changeAnimation("shoot_right",FORWARD);
			attack = 1;
			break;
		}
	}
}