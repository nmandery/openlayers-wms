OpenLayers.WmsMap = OpenLayers.Class(OpenLayers.Map, {
  
  getTitle: function() { return document.title;},

  
  doResize: function( width, height ) {
    document.body.style.width = width;
    map.div.style.width = width;
    document.body.style.height = height;
    map.div.style.height = height;
    
    this.updateSize();
  },

  getProjectionCode: function() {
    if (typeof map.displayProjection === 'undefined') {
      return map.projection.projCode;  
    }
    else {
      return map.displayProjection.projCode;  
    }
  },

  CLASS_NAME: "OpenLayers.JsWmsApi"
})
