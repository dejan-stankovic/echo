#include "TextureAtlasPanel.h"
#include "engine/core/editor/editor.h"
#include "engine/core/editor/qt/QWidgets.h"
#include "engine/core/base/class_method_bind.h"
#include "engine/core/util/PathUtil.h"
#include "engine/core/util/StringUtil.h"
#include "engine/core/io/MemoryReader.h"
#include "engine/core/util/Buffer.h"
#include "engine/core/render/base/image/Image.h"
#include "TextureAtlasPackage.h"

namespace Echo
{
#ifdef ECHO_EDITOR_MODE
	TextureAtlasPanel::TextureAtlasPanel(Object* obj)
	{
		m_textureAtlas = ECHO_DOWN_CAST<TextureAtlas*>(obj);

		m_ui = EditorApi.qLoadUi("engine/core/render/base/editor/panel/TextureAtlasPanel.ui");

		QWidget* splitter = EditorApi.qFindChild(m_ui, "m_splitter");
		if (splitter)
		{
			EditorApi.qSplitterSetStretchFactor(splitter, 0, 0);
			EditorApi.qSplitterSetStretchFactor(splitter, 1, 1);
		}

		// Tool button icons
		EditorApi.qToolButtonSetIcon(EditorApi.qFindChild(m_ui, "m_import"), "engine/core/render/base/editor/icon/import.png");

		// connect signal slots
		EditorApi.qConnectWidget(EditorApi.qFindChild(m_ui, "m_import"), QSIGNAL(clicked()), this, createMethodBind(&TextureAtlasPanel::onImport));
	}

	void TextureAtlasPanel::update()
	{
	}

	void TextureAtlasPanel::onImport()
	{
		if (!m_importMenu)
		{
			m_importMenu = EditorApi.qMenuNew(m_ui);

			EditorApi.qMenuAddAction(m_importMenu, EditorApi.qFindChildAction(m_ui, "m_actionImportFromImages"));
			//EditorApi.qMenuAddAction(m_importMenu, EditorApi.qFindChildAction(m_ui, "m_actionAddSetting"));
			//EditorApi.qMenuAddAction(m_importMenu, EditorApi.qFindChildAction(m_ui, "m_actionAddResource"));

			EditorApi.qConnectAction(EditorApi.qFindChildAction(m_ui, "m_actionImportFromImages"), QSIGNAL(triggered()), this, createMethodBind(&TextureAtlasPanel::onImportFromImages));
			//EditorApi.qConnectAction(EditorApi.qFindChildAction(m_ui, "m_actionAddSetting"), QSIGNAL(triggered()), this, createMethodBind(&TimelinePanel::onAddSetting));
			//EditorApi.qConnectAction(EditorApi.qFindChildAction(m_ui, "m_actionAddResource"), QSIGNAL(triggered()), this, createMethodBind(&TimelinePanel::onAddResource));
		}

		EditorApi.qMenuExec(m_importMenu);
	}

	void TextureAtlasPanel::onImportFromImages()
	{
		StringArray files = Editor::instance()->qGetOpenFileNames(nullptr, "Select Images", "", "*.png");
		if (!files.empty())
		{
			array<i32, 7> sizes = { 64, 128, 256, 512, 1024, 2048, 4096 };
			for (size_t i = 0; i < sizes.size(); i++)
			{
				map<String, int>::type atlaNames;
				TextureAtlasPackage atlasPackage(sizes[i], sizes[i]);
				for (String& file : files)
				{
					String atlaName = PathUtil::GetPureFilename(file, false);

					MemoryReader memReader(file);
					if (memReader.getSize())
					{
						Buffer commonTextureBuffer(memReader.getSize(), memReader.getData<ui8*>(), false);
						Image* image = Image::createFromMemory(commonTextureBuffer, Image::GetImageFormat(file));
						if (image)
						{
							int id = atlasPackage.insert((Color*)(image->getData()), image->getWidth(), image->getHeight());
							atlaNames[atlaName] = id;

							EchoSafeDelete(image, Image);
						}
					}
				}
			}
		}
	}
#endif
}