# Space Invaders
## Classic 2D game for LLP
-------------------

1. **CONTROLS**
2. **GAMEPLAY FEATURES**  
    2.1. Alien Score Values  
    2.2. Score Multiplier  
    2.3. Mega Mode  
    2.4. Life Burn  
    2.5. Extra Lives  
3. **IRRKLANG SOUND ENGINE**

### PERFORMANCE NOTE: For best performance, run the game in Release mode.

## 1. CONTROLS
**W**: 		Life Burn / Menu Selection Up  
**A**: 		Move Left  
**S**: 		Shoot / Menu Selection Down  
**D**: 		Move Right  
**Space**:	Shoot / Menu Accept  
**Enter**:	Shoot / Menu Accept  

## 2. GAMEPLAY FEATURES
### 2.1. Alien Score Values
Top Row 	= 5 * Score Multiplier  
Middle Row	= 2 * Score Multiplier  
Bottom Row	= 1 * Score Multiplier  

### 2.2. Score Multiplier
The score you get from aliens is based on your current Score Multiplier.
Your Score Multiplier starts at 1. Killing aliens increases your Score Multiplier.
Your Score Multiplier is reset to 1 if:  
	- You are hit by an enemy projectile  
	- Your projectile hits the top of the screen  
	- Your projectile hits a barrier  

### 2.3. Mega Mode
Whenever you achieve a Score Multiplier that is a multiple of 20, Mega Mode is activated for 3 seconds.
While Mega Mode is active, you receive the following benefits:  
	- Your projectile turns red and is larger  
	- Your projectile is not destroyed when it hits an alien or a barrier  
	- Your Score Multiplier is not reset if your projectile hits the top of the screen.  

Mega Mode deactivates immediately if you are killed while it is active.

### 2.4. Life Burn
If you have two or more lives, you can press 'W' to expend a life to immediately activate Mega Mode for 3 seconds.

### 2.5. Extra Lives
When you achieve a Score Multiplier that is a multiple of 50, you are rewarded an extra life.

## 3. IRRKLANG SOUND ENGINE
This game uses the irrKlang sound engine.  
Please ensure that you place the included irrKlang .dlls in the appropriate build folder otherwise the game won't run.