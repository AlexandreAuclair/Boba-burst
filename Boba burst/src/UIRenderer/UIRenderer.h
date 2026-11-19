// -- UI_RENDERER_H -- => S'occupe de la création du pipeline 2D --- //

#pragma once

#include "../VulkanContext/bb_vulkanContext.h"

class UIRenderer {

public :
		
	// Constructeur
	UIRenderer(VulkanContext& context, vk::RenderPass renderpass);
	
	// Enregistre les commandes de dessin 2D.
	// Doit être appelé pendant un Render Pass actif
	
	/// <summary>
	/// Dessine l'UI par-dessus la scène 3D
	/// </summary>
	/// <param name="commandBuffer">Command buffer a qui on donne les ordre de dessin</param>
	/// <param name="extent">Dimensions de la surface de rendu en 2D, exprimes en pixels (largeur et hauteur)</param>
	void Draw(vk::CommandBuffer commandBuffer, vk::Extent2D extent);

private : 

	// Creer pipeline graphique rendu UI
	void CreateUIPipeline(vk::RenderPass renderPass);

	// Reference vers contexte vulkan existant
	VulkanContext& m_context;

	// Ressources spécifiques au rendu UI.
	vk::UniquePipelineLayout m_ui_pipelineLayout;
	vk::UniquePipeline m_ui_pipeline;
};