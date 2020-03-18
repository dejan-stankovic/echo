#pragma once

#include "../ShaderProgram.h"
#include "panel/RenderPipelinePanel.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
    class RenderPipelineEditor : public ObjectEditor
    {
    public:
        RenderPipelineEditor(Object* object);
        virtual ~RenderPipelineEditor();
        
        // get icon, used for editor
        const char* getEditorIcon() const;

		// on editor select this node
		virtual void onEditorSelectThisNode() override;

        // update self
        void editor_update_self();
        
    private:
        RenderpipelinePanel* m_panel = nullptr;
    };
    
#endif
}
