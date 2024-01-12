// canvas.js
$(document).ready(function() {

    var button_radius = 30;
    var stickerButtonName = ['star', 'heart', 'cake', 'light'];  // name of images in img_source, respectively
    var thickButton = [];  // Array to store all thickness (circle)
    var strokeSize = [];
    var geoButtonName = ['lineButton', 'rectangleButton', 'circleButton'];

    var change_flag = false;
    var undo_flag = false;
    var start_flag = false;
    var addStickerFlag = [false,false,false,false,false];

    var sticker_count = {a:0};
    var geo_count = {a:0};
    
    var allSet = [];
    var colors = [];  // Array to store all colors (circle), and eraser
    var newStickers = [];

    var pathPoints = []; // Stores the path of the mouse
    var pathSet = [];  // stores the userPath(s)
    var isPainting = false;
    var before_pathPoints = 0;
 
    var pen_opacity = {a:1.0}; // Set the opacity of pen
    var geo_button = 'none';
	
    var startX, startY, endX, endY;
    
    create_tool(button_radius, stickerButtonName, colors, thickButton,strokeSize);
    create_pointer();

    // SETUP AND OBTAIN DATA FROM LEAP MOTION
    Leap.loop({}, function(frame) {
        if (frame.pointables.length > 0) {
            var leapCursorLayer = $('canvas').getLayer('leapCursor');
            var leapCursorPointer = $('canvas').getLayer('leapCursor_pointer');
            var saveButtonLayer = $('canvas').getLayer('saveButton');
            leapCursorLayer.visible = true;
            leapCursorPointer.visible = true;

            var pointable = frame.pointables[1];

            //食指的位置
            var pointerOnCanvas = {
                x: (frame.pointables[1].tipPosition[0]+200) * 3,
                y: (frame.pointables[1].tipPosition[2]+100) * 3
            };        
            leapCursorLayer.x = pointerOnCanvas.x;
            leapCursorLayer.y = pointerOnCanvas.y;
            leapCursorPointer.x = pointerOnCanvas.x;
            leapCursorPointer.y = pointerOnCanvas.y;

            var hand = frame.hands[0];

             // 存檔判斷
             if (collisionTest(leapCursorLayer, saveButtonLayer)) {
                if (!saveButtonLayer.hoverStartTime) {
                    saveButtonLayer.hoverStartTime = new Date().getTime();
                } else if (new Date().getTime() - saveButtonLayer.hoverStartTime > 2000) {
                    saveCanvas(880, 470); // Call the save function
                    saveButtonLayer.hoverStartTime = null; // Reset the timer
                }
            } else {
                saveButtonLayer.hoverStartTime = null;
            }

            //add
            for(var i = 0; i < geoButtonName.length; ++i){
                if (collisionTest(leapCursorLayer, $('canvas').getLayer(geoButtonName[i])) && hand.grabStrength == 1 && $('canvas').getLayer(geoButtonName[i]).fillStyle == '#aa00ff')
                    geo_button = geoButtonName[i];
                $('canvas').getLayer(geoButtonName[i]).fillStyle='#aa00ff';
            }
            //把碰到的按鈕變黑色
            for(var i = 0; i < geoButtonName.length; ++i){
                if(geoButtonName[i] == geo_button)
                    $('canvas').getLayer(geoButtonName[i]).fillStyle='#000';
            }

            //把按鈕關掉
            for(var i = 0; i < geoButtonName.length; ++i){
                var button = $('canvas').getLayer(geoButtonName[i]);
                if (collisionTest(leapCursorLayer, button) && hand.grabStrength === 0 && button.fillStyle == '#000') {
                    geo_button = 'none';
                    button.fillStyle='#aa00ff';
                }
            }

            for(var i = 0; i < geoButtonName.length; ++i){
                if (hand.grabStrength === 0 && start_flag == false && geo_button != 'none') { //手張開，起點
                    startX=pointerOnCanvas.x;
                    startY=pointerOnCanvas.y;
                    start_flag = true;// 控制只更新一次開始位置
                    $('canvas').drawArc({
                        fillStyle: $('canvas').getLayer('leapCursor_pointer').fillStyle,
                        x: startX,
                        y: startY,
                        radius: 5,
                        layer: true,
                        name: 'dot',
                    });
                    allSet.push('dot');
                }   
            }
            //draw geo shape
            for(var i = 0; i < geoButtonName.length; ++i){
                if (hand.grabStrength === 1 && start_flag == true && geo_button == geoButtonName[i] && !collisionTest(leapCursorLayer, $('canvas').getLayer('lineButton')) 
                && !collisionTest(leapCursorLayer, $('canvas').getLayer('rectangleButton')) && !collisionTest(leapCursorLayer, $('canvas').getLayer('circleButton'))) { // 手握拳，終點，並劃出直線
                    allSet.pop();
                    endX=pointerOnCanvas.x;
                    endY=pointerOnCanvas.y;
                    draw_geo_shape(startX,startY,endX,endY, geo_count, allSet, geoButtonName[i]);
                    start_flag = false;
                    $('canvas').removeLayer('dot');
                }
            }
            console.log(allSet);
         
            // undo path
            if(collisionTest(leapCursorLayer, $('canvas').getLayer('undoButton')) ){
                if( hand.grabStrength === 1.0 && undo_flag == 0 )
                    undo_flag = undoPath(allSet);
            }else{
                undo_flag = 0;
            }
            // reset all
            if(hand.grabStrength === 1.0 && collisionTest(leapCursorLayer, $('canvas').getLayer('resetButton'))){
                resetAll(allSet, newStickers);
                allSet = [];
                newStickers = [];
            }
            // HL / pen
            if(collisionTest(leapCursorLayer, $('canvas').getLayer('HLButton')) ){
                if( hand.grabStrength === 1.0 && change_flag == 0 )
                    change_flag = change_pen_type(pen_opacity);
            }else{
                change_flag = false;
            }

            for (var i=0 ; i<colors.length ; ++i){
                if (hand.grabStrength === 1.0 && collisionTest($('canvas').getLayer('leapCursor_pointer'),colors[i]))
                    change_pointer_color(colors[i].fillStyle);  //  在調色盤上握拳 -> pointer變成該顏色       
            }
            
            for (var i=0 ; i<thickButton.length ; ++i){
                if (hand.grabStrength === 1.0 && collisionTest($('canvas').getLayer('leapCursor_pointer'),thickButton[i]))
                    change_pen_thick(strokeSize[i].radius);        
            }
            //  只伸出食指 -> 畫畫
            if (hand.indexFinger.extended && !hand.thumb.extended && !hand.middleFinger.extended 
                && !hand.ringFinger.extended && !hand.pinky.extended){
                if(isPainting == false){
                    isPainting = true;
                    addUserPathLayer(pathSet.length, pen_opacity);
                    pathSet.push($('canvas').getLayer(`userPath${pathSet.length}`));
                    allSet.push(`userPath${pathSet.length-1}`);
                }
                paint(pathSet, pathPoints, pointerOnCanvas, before_pathPoints);
            }else if (isPainting == true) {
                before_pathPoints = pathPoints.length;
                isPainting = false;
            }

            for(var i = 0; i < stickerButtonName.length; ++i){
                var sticker_button = $('canvas').getLayer(stickerButtonName[i]);
                if(collisionTest(leapCursorLayer, sticker_button) && !addStickerFlag[i]) 
                    addStickerFlag[i] = add_sticker(sticker_button, newStickers, sticker_count);
                if(!collisionTest(leapCursorLayer, sticker_button))
                    addStickerFlag[i] = false;
            } 

            for(var i = 0; i < newStickers.length; ++i){
                if (collisionTest(leapCursorLayer, $('canvas').getLayer(newStickers[i])) ) {
                    if (hand.pinchStrength > 0.9){  // 捏(拇指與任何一指接觸)
                        move(newStickers[i], leapCursorLayer.x, leapCursorLayer.y);
                        break;
                    }else{
                        resize($('canvas').getLayer(newStickers[i]), hand);
                    }
                }
            }
            for(var i = 0; i < newStickers.length; ++i){
                if(collisionTest($('canvas').getLayer(newStickers[i]), $('canvas').getLayer('trashCan'))){
                    $('canvas').removeLayer(newStickers[i]);  
                    newStickers.splice(i, 1);
                    break;    
                }       
            }
            $('canvas').setLayer('leapxy', { text: '(' + pointerOnCanvas.x.toFixed() + ', ' + pointerOnCanvas.y.toFixed() + ')' });
            $('canvas').drawLayers();
        } else {
            $('canvas').setLayer('leapxy', { text: 'No Finger!' });
            $('canvas').setLayer('leapCursor', { visible: false }).drawLayers();
        }
    }); 
});


function create_tool(button_radius, stickerButtonName, colors, thickButton, strokeSize){
    create_sticker_button(stickerButtonName);
    create_geometric_button(button_radius);
    create_save_button(button_radius);
    create_delete_button(button_radius);
    create_thickness_button(button_radius, thickButton, strokeSize);
    create_color_button(button_radius, colors);
    create_trashCan();
}

