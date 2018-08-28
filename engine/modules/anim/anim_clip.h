#pragma once

#include "anim_node.h"

namespace Echo
{
	struct AnimClip
	{
		String					m_name;
		vector<AnimNode*>::type	m_nodes;
		float					m_time;
		float					m_length;

		AnimClip() : m_time(0.f), m_length(1.f)
		{}

		// update
		void update( float deltaTime, Node* base)
		{
			m_time += deltaTime;
			if (m_time > m_length)
				m_time = 0.f;

			for (AnimNode* animNode : m_nodes)
			{
				animNode->updateToTime(m_time, base);
			}
		}
	};
}