//"use strict";

var LibraryDLM = {
    $gfxContext: null,
    $DLM: {
        audioCtx: null,
        audioBuffer: [],
        audioSources: [],
        bgmPlayer: null,
        currentBgm: 0,
        mirror: false,
        images: [],
        fontSize: 14,
        fontType: 0,
    },

    //------------------------------------------------------------------------------
    // Graphics
    //------------------------------------------------------------------------------
    
    graphics_init: function() {
        var canvas = Module['canvas'];
        gfxContext = canvas.getContext('2d');
        gfxContext.textBaseline = 'top';
        gfxContext.strokeStyle = 'black';
    },

    loadimage: function(filename) {
        filename = UTF8ToString(filename);
        var img = new Image();
        img.src = filename;
        DLM.images.push({
            img: img,
            x: 0,
            y: 0,
            w: 1,
            h: 1,
        });
        return DLM.images.length - 1;
    },

    subimage: function(x, y, w, h, img) {
        var src = DLM.images[img];
        DLM.images.push({
            img: src.img,
            x: x,
            y: y,
            w: w,
            h: h
        });
        return DLM.images.length - 1;
    },

    getimagesize: function(img, pw, ph) {
        var src = DLM.images[img];
        setValue(pw, src.w, 'i32');
        setValue(ph, src.h, 'i32');
    },

    clearscreen: function() {
        gfxContext.fillRect(0, 0, 480, 420);
    },

    drawline: function(x, y, w, h) {
        gfxContext.beginPath();
        gfxContext.moveTo(x, y);
        gfxContext.lineTo(w, h);
        gfxContext.closePath();
        gfxContext.stroke();
    },

    drawrect: function(x, y, w, h) {
        gfxContext.strokeRect(x, y, w, h);
    },

    fillrect: function(x, y, w, h) {
        gfxContext.fillRect(x, y, w, h);
    },

    drawarc: function(x, y, w, h) {
        gfxContext.lineWidth = 0.5;
        gfxContext.strokeStyle = 'black';
        gfxContext.arc(x, y, w, 0, Math.PI * 2);
        gfxContext.stroke();
    },

    fillarc: function(x, y, w, h) {
        gfxContext.beginPath();
        gfxContext.arc(x, y, w, 0, Math.PI * 2);
        gfxContext.closePath();
        gfxContext.fill();
    },


    drawimage: function(img, x, y) {
        var src = DLM.images[img];
        if (!src)
            return;
        
        gfxContext.fillStyle = 'white';
        if (DLM.mirror) {
            gfxContext.save();
            gfxContext.translate(x + src.w, y);
            gfxContext.scale(-1, 1);
            gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, 0, 0, src.w, src.h);
            gfxContext.restore();
        }
        else {
            gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, x, y, src.w, src.h);
        }
    },

    drawimageflip: function(img, x, y) {
        var src = DLM.images[img];
        if (!src)
            return;
        
        gfxContext.fillStyle = 'white';
        gfxContext.save();
        gfxContext.translate(x, y + src.h);
        gfxContext.scale(1, -1);
        gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, 0, 0, src.w, src.h);
        gfxContext.restore();
    },

    setfont: function (size, thick) {
        DLM.fontSize = size;
    },

    setfonttype: function(type) {
        DLM.fontType = type;
    },

    drawstring: function(x, y, str) {
        str = UTF8ToString(str);
        gfxContext.font = DLM.fontSize + 'px sans-serif';
        if (DLM.fontType == 1) {
            var f = gfxContext.fillStyle;
            gfxContext.fillStyle = 'black';
            gfxContext.fillText(str, x, y - 1);
            gfxContext.fillText(str, x, y + 1);
            gfxContext.fillText(str, x - 1, y);
            gfxContext.fillText(str, x + 1, y);
            gfxContext.fillStyle = f;

        }
        gfxContext.fillText(str, x, y);
    },

    setcolor: function(r, g, b) {
        gfxContext.fillStyle = 'rgba(' + r + ',' + g + ',' + b + ',' + 255 + ')';
    },

    setmirror: function(mirror) {
        DLM.mirror = (mirror != 0);
    },

    begindraw: function() {
    },

    enddraw: function() {
        var canvas = Module['canvas'];
        var h = canvas.height;
        if (h <= 420)
            return;

        var drawArrow = function () {
        }

        gfxContext.clearRect(0, 420, canvas.width, h - 420);
        gfxContext.save();

        var key = [];
        key[0] = (DLM.touch & 1) != 0;
        key[1] = (DLM.touch & 8) != 0;
        key[2] = (DLM.touch & 2) != 0;
        key[3] = (DLM.touch & 4) != 0;

        gfxContext.fillStyle = 'white';
        gfxContext.strokeStyle = 'black';
        gfxContext.strokeWidth = 2;
        gfxContext.translate(140, 420 + (h - 420) / 2);
        for (var i = 0; i < 4; i++) {
            gfxContext.globalAlpha =  key[i] ? 0.6 : 0.3;
            gfxContext.beginPath();
            gfxContext.moveTo(-5, 0);
            gfxContext.lineTo(-35, -30);
            gfxContext.lineTo(-80, -30);
            gfxContext.lineTo(-80, 30);
            gfxContext.lineTo(-35, 30);
            gfxContext.closePath();
            gfxContext.fill();
            gfxContext.stroke();
            gfxContext.rotate(Math.PI / 2);
        }

        gfxContext.setTransform(1, 0, 0, 1, 340, 420 + (h - 420) / 2);
        gfxContext.globalAlpha = (DLM.touch & 0x10) != 0 ? 0.6 : 0.3;
        gfxContext.fillRect(-40, -40, 80, 80);
        gfxContext.strokeRect(-40, -40, 80, 80);

        gfxContext.restore();
    },

    //------------------------------------------------------------------------------
    // Input
    //------------------------------------------------------------------------------
    input_init: function() {
        DLM.key = 0;
        DLM.touch = 0;
        DLM.mouse = 0;

        var canvas = Module['canvas'];
        var key_callback = function (e) {
            e.preventDefault();
            if (e.repeat)
                return;

            var buttons = 0;
            switch (e.keyCode) {
                case 37: buttons |= 1; break;
                case 38: buttons |= 16; break;
                case 39: buttons |= 2; break;
                case 40: buttons |= 4; break;
                case 13: buttons |= 32; break;
            }

            if (e.type == 'keydown') {
                DLM.key |= buttons;
            }
            else if (e.type == 'keyup') {
                DLM.key = DLM.key & (~buttons);
            }
        };

        var mouse_callback = function (e) {
            switch (e.type) {
                case 'mousedown': DLM.mouse |= 32; break;
                case 'mouseup': DLM.mouse &= (~32); break;
            }
        }

        var check_touch = function (x, y, x1, y1, x2, y2) {
            return (x > x1 && x < x2 && y > y1 && y < y2);
        }

        var touch_callback = function (e) {
            e.preventDefault();
            var touch = 0;
            var centerY = 420 + (canvas.height - 420) / 2;
            var rect = canvas.getBoundingClientRect();

            for (var i = 0; i < e.touches.length; i++) {
                var t = e.touches.item(i);
                var x = (t.clientX - rect.left) / (rect.right - rect.left) * canvas.width;
                var y = (t.clientY - rect.top) / (rect.bottom - rect.top) * canvas.height;

                if (check_touch(x, y, 140 - 40, centerY + 20, 140 + 40, centerY + 100)) touch |= 4;
                else if (check_touch(x, y, 140 - 40, centerY - 100, 140 + 40, centerY - 40)) touch |= 8;
                else if (check_touch(x, y, 140 - 100, centerY - 40, 140, centerY + 40)) touch |= 1;
                else if (check_touch(x, y, 140, centerY - 40, 140 + 100, centerY + 40)) touch |= 2;

                if (check_touch(x, y, 340 - 100, centerY - 100, 340 + 100, centerY + 100)) touch |= 0x10;
                if (check_touch(x, y, 0, 0, 480, 420)) touch |= 0x20;
            }
            DLM.touch = touch;
        }

        canvas.addEventListener('keydown', key_callback, true);
        canvas.addEventListener('keyup', key_callback, true);
        canvas.addEventListener('mousedown', mouse_callback, true);
        canvas.addEventListener('mouseup', mouse_callback, true);
        canvas.addEventListener('touchstart', touch_callback, true);
        canvas.addEventListener('touchend', touch_callback, true);
        canvas.addEventListener('touchmove', touch_callback, true);
        canvas.addEventListener('touchcancel', touch_callback, true);
    },

    input_waitkey: function() {
    },

    input_get: function() {
        return DLM.key | DLM.touch | DLM.mouse;
    },

    getrand: function(maxValue) {
        return Math.floor(Math.random() * maxValue);
    },

    gettime: function() {
        var t = new Date().getTime();
        return t % 0xfffffff;
    },

    //------------------------------------------------------------------------------
    // Sound
    //------------------------------------------------------------------------------

    sound_init: function() {
        return;
        var audioCtx;
        try {
            audioCtx = new (window.AudioContext || window.webkitAudioContext)();
        }
        catch (e) {
        }

        if (audioCtx) {
            DLM.audioCtx = audioCtx;

            // Old Web Audio API (e.g. Safari 6.0.5) had an inconsistently named createGainNode function.
            if (typeof (audioCtx.createGain) === 'undefined') audioCtx.createGain = audioCtx.createGainNode;

            var loadAudio = function (id, url) {
                var xhr = new XMLHttpRequest();
                xhr.open('GET', url, true);
                xhr.responseType = 'arraybuffer';
                xhr.onload = function (e) {
                    audioCtx.decodeAudioData(this.response, function (buffer) {
                        DLM.audioBuffer[id] = buffer;
                    });
                };
                xhr.send();
            }

            loadAudio(1, 'bgm/se1.mp3');
            loadAudio(2, 'bgm/se2.mp3');
        }

        var player = document.createElement("AUDIO");
        if (player) {
            player.volume = 0.5;
            DLM.bgmPlayer = player;
        }
    },

    soundplay: function(x) {
        var i = 0;
        var s = 0;
        var d = 0;
        switch (x) {
            case 1: i = 1; s = 0.000; d = 0.641; break;
            case 3: i = 1; s = 0.700; d = 0.432; break;
            case 4: i = 1; s = 1.200; d = 0.928; break;
            case 5: i = 1; s = 2.200; d = 0.458; break;
            case 6: i = 1; s = 2.700; d = 0.249; break;
            case 7: i = 1; s = 3.000; d = 0.928; break;
            case 8: i = 1; s = 4.000; d = 0.928; break;
            case 9: i = 1; s = 5.000; d = 0.928; break;
            case 10: i = 1; s = 6.000; d = 0.432; break;
            case 11: i = 1; s = 6.500; d = 6.936; break;
            case 12: i = 1; s = 13.500; d = 3.253; break;

            case 13: i = 2; s = 0.000; d = 0.275; break;
            case 14: i = 2; s = 0.300; d = 0.118; break;
            case 15: i = 2; s = 0.500; d = 0.797; break;
            case 16: i = 2; s = 1.400; d = 4.428; break;
            case 17: i = 2; s = 5.900; d = 6.936; break;
            case 18: i = 2; s = 13.000; d = 1.476; break;
        }

        if (i > 0) {
            var audioCtx = DLM.audioCtx;
            if (audioCtx == null)
                return;

            var buf = DLM.audioBuffer[i];
            if (buf) {
                var source = audioCtx.createBufferSource();
                source.buffer = buf;
                source.connect(audioCtx.destination);
                source.start(0, s, d);
                source.onended = function () {
                    if (DLM.audioSources[x] == source) {
                        DLM.audioSources[x] = null;
                    }
                }
                DLM.audioSources[x] = source;
            }
        }
    },

    soundstop: function(x) {
        var s = DLM.audioSources[x];
        if (s) {
            s.stop();
            DLM.audioSources[x] = null;
        }
    },

    soundcheck: function(x) {
        return DLM.audioSources[x] != null;
    },

    bgmchange: function (x) {
        DLM.currentBgm = x;
    },

    bgmstop: function (x) {
        if (DLM.bgmPlayer) {
            DLM.bgmPlayer.pause();
        }
    },

    bgmplay: function(x) {
        var player = DLM.bgmPlayer;
        if (!player) return;

        switch (DLM.currentBgm) {
            case 100: player.src = "bgm/field.mp3"; break;
            case 103: player.src = "bgm/dungeon.mp3"; break;
            case 104: player.src = "bgm/star4.mp3"; break;
            case 105: player.src = "bgm/castle.mp3"; break;
            case 106: player.src = "bgm/puyo.mp3"; break;
            default: return;
        }

        player.loop = true;
        player.play();
    },

    //------------------------------------------------------------------------------
    // AD
    //------------------------------------------------------------------------------
    adshow: function() {
        console.log('ADSHOW');
        var ad = document.getElementById('ad');
            console.log(ad);
        if (ad) {
            DLM.ad = ad;
            console.log(ad);
        }

        /*
        if (!DLM.ad) {
            var ad = document.createElement('DIV');
            DLM.ad = ad;
            document.body.appendChild(ad);
            ad.innerHTML = '<script type="text/javascript">var cpro_id = "u2379481"</script><script src="http://cpro.baidustatic.com/cpro/ui/cm.js" type="text/javascript"></script>';
            ad.style.position = 'absolute';
            ad.style.color = 'white';
            ad.style.padding = '0';
            ad.style.margin = '0';

            var sc = document.createElement('SCRIPT');
            sc.src = "http://cpro.baidustatic.com/cpro/ui/cm.js";
            sc.type = "text/javascript";

            ad.appendChild(sc);
        }
        */
        if (DLM.ad) {
            var ad = DLM.ad;
            var canvas = Module['canvas'];
            var rect = canvas.getBoundingClientRect();
            ad.style.left = rect.left + 'px';
            ad.style.top = rect.top + 'px';
            ad.style.width = rect.width + 'px';
            ad.style.height = 100;
            DLM.ad.hidden = false;
        }
    },

    adhide: function() {
        if (DLM.ad) {
            DLM.ad.hidden = true;
        }
    }
};
autoAddDeps(LibraryDLM, '$DLM', '$gfxContext');
mergeInto(LibraryManager.library, LibraryDLM);
