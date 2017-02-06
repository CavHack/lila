var LibraryHeisenberg = {
	heisenbergOnHeadersDecoded: function(){
	par_heisenbergOnHeadersDecoded();
},
	heisenbergOnPictureDecoded: function($buffer, width, height) {
	par_heisenbergOnPictureDecoded($buffer, width, height);
}, 
	extern_emit_image: function($buffer, width, height, $otherBuffer){
	par_heisenbergOnPictureDecoded($buffer, width, height, $otherBuffer);
	}

};

mergeInto(LibraryManager.library, LibraryHeisenberg)
