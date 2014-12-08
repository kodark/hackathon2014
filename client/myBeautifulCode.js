    var players = {};
    var playersSprite = {};
    var playersName = {};
    var bullets = {};
    var bulletsSprite = {};
    var HPsprite = {};
    var maxVelocity = 6;
	var realposition = 
	{ 
	x: 0,
	y: 0
	}
	var myname = "";
	var func = "";
	var webSocket = undefined;
	
function start()
{
    webSocket = new WebSocket('ws://devhead.koding.io:1337');
    webSocket.onopen = function(event) {
        game();
    };
    webSocket.onclose = function(event) {
        alert('onclose');
    };
}

function game()
{
    var myname = document.getElementById('username').value;
    var func = document.getElementById('function').value;
    webSocket.send(1 + ' ' + func + ' ' + myname);
    
    var blurFilter = new PIXI.BlurFilter();
    var stage = new PIXI.Stage(0x000000, true);
    var BGtexture = PIXI.Texture.fromImage("./background.jpg");
    var BulletTexture = PIXI.Texture.fromImage("./bullet.png");
    var BGsprite = new PIXI.Sprite(BGtexture);
    BGsprite.filters = [blurFilter];
    BGsprite.anchor.x = 0.5;
    BGsprite.anchor.y = 0.5;
    BGsprite.position.x = window.innerWidth / 2;
    BGsprite.position.y = window.innerHeight / 2;
    stage.addChild(BGsprite);
    
    var renderer = PIXI.autoDetectRenderer(window.innerWidth, window.innerHeight, null);

    document.body.appendChild(renderer.view);
    stage.interactive = true;
    renderer.view.style.position = "absolute";
    renderer.view.style.top = "0px";
    renderer.view.style.left = "0px";
    
    
    
    var texture = PIXI.Texture.fromImage("./player.png");
    var HPtexture = PIXI.Texture.fromImage("./hp.png");
    
    var playerRunner = new PIXI.Sprite(texture);

    playerRunner.interactive = true;
    playerRunner.buttonMode = true;

    playerRunner.anchor.x = 0.5;
    playerRunner.anchor.y = 0.5;
        
    playerRunner.scale.x = 0.2;
    playerRunner.scale.y = -0.2;
	playerRunner.position.x = window.innerWidth / 2;
	playerRunner.position.y = window.innerHeight / 2;


   playerRunner.runRabbitRun = function(deltaX, deltaY)
        {
            var oldposition =
            {
                x: (realposition.x + 0),
                y: (realposition.y + 0)
            };
            var mouse = stage.interactionManager.mouse;
            var abs_vector = Math.sqrt((mouse.global.x - window.innerWidth/2) * (mouse.global.x - window.innerWidth/2) + (mouse.global.y - window.innerHeight / 2) * (mouse.global.y - window.innerHeight / 2));
            if (deltaX * deltaY > 0)
            {
            realposition.x +=  maxVelocity * deltaX * (mouse.global.x - window.innerWidth / 2) / abs_vector;
            realposition.y +=  maxVelocity * deltaY * (mouse.global.y - window.innerHeight / 2) / abs_vector;
            }
            else
            {
            realposition.x +=  maxVelocity * deltaX * (mouse.global.y - window.innerHeight / 2) / abs_vector;
            realposition.y +=  maxVelocity * deltaY * (mouse.global.x - window.innerWidth/2) / abs_vector;
            }
            if ((mouse.global.y - window.innerHeight / 2) !== 0)
            {
                this.rotation = Math.atan((mouse.global.x - window.innerWidth/2) / (- mouse.global.y + window.innerHeight / 2));
                if ((mouse.global.y - window.innerHeight / 2) > 0)
                {
                        this.rotation += Math.PI;
                } 
            }
            else
            {
            if ((mouse.global.x - window.innerWidth / 2) < 0)
                    {
                        this.rotation = - Math.PI / 2;
                    }
                    else
                    {
                        this.rotation = Math.PI / 2;
                    }
            }
			
			if (realposition.x < 10) realposition.x = 10;
			if (realposition.y < 10) realposition.y = 10;
			if (realposition.x > 1990) realposition.x = 1990;
			if (realposition.y > 1990) realposition.y = 1990;
			
			BGsprite.position.x = window.innerWidth / 2 + 1000 - realposition.x;
			BGsprite.position.y = window.innerHeight / 2 + 1000 - realposition.y;
			
			for (var i in playersSprite)
			{
			    if (playersSprite.hasOwnProperty(i))
			    {
			        playersSprite[i].position.x += - realposition.x + oldposition.x;
			        playersSprite[i].position.y += - realposition.y + oldposition.y;
			    }
			}
			
			for (var i in HPsprite)
			{
			    if (HPsprite.hasOwnProperty(i))
			    {
			        HPsprite[i].position.x += - realposition.x + oldposition.x;
			        HPsprite[i].position.y += - realposition.y + oldposition.y;
			    }
			}
			
			for (var i in playersName)
			{
			    if (playersName.hasOwnProperty(i))
			    {
			        playersName[i].position.x += - realposition.x + oldposition.x;
			        playersName[i].position.y += - realposition.y + oldposition.y;
			    }
			}
			
            webSocket.send(2 + ' ' + realposition.x + ' ' + realposition.y + ' ' + this.rotation);			
            }
        
    playerRunner.rotateRabbitRotate = function()
        {
            var mouse = stage.interactionManager.mouse;
            var abs_vector = Math.sqrt((mouse.global.x - window.innerWidth/2) * (mouse.global.x - window.innerWidth/2) + (mouse.global.y - window.innerHeight / 2) * (mouse.global.y - window.innerHeight / 2));
            if ((mouse.global.y - window.innerHeight / 2) !== 0)
            {
                this.rotation = Math.atan((mouse.global.x - window.innerWidth/2) / (- mouse.global.y + window.innerHeight / 2));
                if ((mouse.global.y - window.innerHeight / 2) > 0)
                {
                    this.rotation += Math.PI;   
                } 
            }
            else
            {
            if ((mouse.global.x - window.innerWidth/2) < 0)
                    {
                        this.rotation = - Math.PI / 2;
                    }
                    else
                    {
                        this.rotation = Math.PI / 2;
                    }
            }
            if (webSocket.readyState == webSocket.OPEN)
            {
                webSocket.send(2 + ' ' + realposition.x + ' ' + realposition.y + ' ' + this.rotation);
             //  console.log(2 + ' ' + realposition.x + ' ' + realposition.y + ' ' + this.rotation + ' SENDING BLYA');
            }
        }
    
    playerRunner.mousemove = function(data)
    {
        playerRunner.rotateRabbitRotate();
    }
    
//    realposition.x = Math.random() * 2000;
//    realposition.y = Math.random() * 2000;
	
	realposition.x = 1000;
    realposition.y = 1000;
        
    stage.addChild(playerRunner);
    
    
    playerRunner.rotateRabbitRotate();
    
window.addEventListener('keydown', function(event) {
 if (event.keyCode === 38 || event.keyCode === 87) {
      playerRunner.runRabbitRun(1, 1);
  }
  
   if (event.keyCode === 40 || event.keyCode === 83) {
      playerRunner.runRabbitRun(-1, -1);
  }
  
   if (event.keyCode === 37 || event.keyCode === 65) {
      playerRunner.runRabbitRun(1, -1);
  }
  
   if (event.keyCode === 39 || event.keyCode === 68) {
      playerRunner.runRabbitRun(-1, 1);
  }
  
  
}, false);
    
webSocket.onmessage = function(e)
{
    strings = e.data.split(' ');
     var id = parseInt(strings[1], 10);
//     console.log(strings);
    switch (parseInt(strings[0], 10))
    {
        case 4:
           console.log(strings);
 //           console.log('id =' + id);
            
                players[id] = {
                    x: 0, 
                    y: 0,
                    a: 0,
                    hp: 100,
                    name: strings[2]
                }
 //           console.log(players[id]);
                playersSprite[id] = new PIXI.Sprite(texture);
                playersSprite[id].tint = Math.random() * 0xFFFFFF;
                playersSprite[id].position.x = players[id].x;
                playersSprite[id].position.y = players[id].y;
                playersSprite[id].rotation = players[id].a;
                playersName[id] = new PIXI.Text(players[id].name, { font: "12px Snippet", fill: "white", align: "center" });
                playersName[id].anchor.x = 0.5;
                playersName[id].anchor.y = 2;
                playersSprite[id].anchor.x = 0.5;
                playersSprite[id].anchor.y = 0.5;
                playersSprite[id].scale.x = 0.2;
                playersSprite[id].scale.y = -0.2;
                HPsprite[id] = new PIXI.Sprite(HPtexture);
                HPsprite[id].position.x = players[id].x;
                HPsprite[id].position.y = players[id].y;
                HPsprite[id].scale.x = 10;
                HPsprite[id].anchor.x = 0.5;
                HPsprite[id].anchor.y = 3;
                
                stage.addChild(playersSprite[id]);
                stage.addChild(playersName[id]);
                stage.addChild(HPsprite[id]);
                break;
            
             case 5:
//              console.log(strings);
 //               console.log('id =' + id);
  //             console.log(players[id]);
  //             console.log(playersSprite[id]);
                
               players[id].x = playersSprite[id].position.x = playersName[id].position.x = HPsprite[id].position.x = parseFloat(strings[2], 10) - realposition.x + window.innerWidth / 2;
               players[id].y = playersSprite[id].position.y = playersName[id].position.y = HPsprite[id].position.y = parseFloat(strings[3], 10) - realposition.y + window.innerHeight / 2;
               players[id].a = playersSprite[id].rotation = parseFloat(strings[4], 10);
                
                
                break;
            case 6:
                stage.removeChild(HPsprite[id]);
                stage.removeChild(playersSprite[id]);
                stage.removeChild(playersName[id]);
                delete HPsprite[id];
                delete playersSprite[id];
                delete playersName[id];
                delete players[id];
                break;
                
            case 7:
          //    console.log(strings);
 //               console.log('id =' + id);
  //             console.log(players[id]);
  //             console.log(playersSprite[id]);
            if (bullets.hasOwnProperty(id))
            {
               bullets[id].x = bulletsSprite[id].position.x = parseFloat(strings[3], 10) - realposition.x + window.innerWidth / 2;
               bullets[id].y = bulletsSprite[id].position.y = parseFloat(strings[4], 10) - realposition.y + window.innerHeight / 2;
            //   console.log("Old bullet: " + bullets[id].x + ' ' + bullets[id].y);
            }
            else
            {
                bullets[id] = {
                    x: parseFloat(strings[3], 10) - realposition.x + window.innerWidth / 2,
                    y: parseFloat(strings[4], 10) - realposition.y + window.innerHeight / 2
                };
            //    console.log("New bullet: " + bullets[id].x + ' ' + bullets[id].y);
                bulletsSprite[id] = new PIXI.Sprite(BulletTexture);
                bulletsSprite[id].interactive = true;
                bulletsSprite[id].buttonMode = true;
                if (playersSprite[parseInt(strings[2], 10)] === undefined)
                    bulletsSprite[id].tint = 0xFFFFFF;
                else
                    bulletsSprite[id].tint = playersSprite[parseInt(strings[2], 10)].tint;
                bulletsSprite[id].position.x = bullets[id].x;
                bulletsSprite[id].position.y = bullets[id].y;
                bulletsSprite[id].anchor.x = 0.5;
                bulletsSprite[id].anchor.y = 0.5;
                bulletsSprite[id].scale.x = 0.05;
                bulletsSprite[id].scale.y = 0.05;
                stage.addChild(bulletsSprite[id]);
            }
            break;   
            case 8:
//              console.log(strings);
 //               console.log('id =' + id);
  //             console.log(players[id]);
  //             console.log(playersSprite[id]);
                
               stage.removeChild(bulletsSprite[id]);
               delete bulletsSprite[id];
               delete bullets[id];
            break;
            
            case 9:
//              console.log(strings);
 //               console.log('id =' + id);
  //             console.log(players[id]);
  //             console.log(playersSprite[id]);
                realposition.x = parseFloat(strings[1], 10);
                realposition.y = parseFloat(strings[2], 10);
               
            break;
            
            case 10:
              console.log(strings);
 //               console.log('id =' + id);
  //             console.log(players[id]);
  //             console.log(playersSprite[id]);
                players[id].hp = parseInt(strings[2], 10);
                console.log("New HP = " + players[id].hp);
                if (HPsprite.hasOwnProperty(id))
                {
                    HPsprite[id].scale.x = players[id].hp / 10;
                    console.log("New HP scale.x = " + HPsprite[id].scale.x);
                }

               
            break;
    }
};
    
    function animate() {
        /*
        for (var i in playersSprite)
            if (playersSprite.hasOwnProperty(i))
            {
                playersSprite[i].position.x = players[i].x;
                playersSprite[i].position.y = players[i].y;
                playersSprite[i].rotation = players[i].a;
                playersName[i].position.x = players[i].x;
                playersName[i].position.y = players[i].y;
            }
            */
        
     //   playerRunner.rotateRabbitRotate();
        renderer.render(stage);
        requestAnimFrame(animate);
    }
    
    stage.click = function (e) {
        if (webSocket.readyState == webSocket.OPEN)
        {
//       webSocket.send(3 + ' ' + playerRunner.position.x + ' ' + playerRunner.position.y + ' ' + playerRunner.rotation);
        webSocket.send(3);
        console.log("SHOOT!!!");
        }
            
    };
    
    requestAnimFrame(animate);
}