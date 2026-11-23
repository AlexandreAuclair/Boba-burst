// -- UI_RENDERER_H -- => S'occupe de la création du pipeline 2D --- //

#pragma once

#include "../VulkanContext/bb_vulkanContext.h"
#include "PauseMenuGeometry.h"

class UIRenderer {

public :
		
	// Constructeur
	UIRenderer(VulkanContext& context, vk::RenderPass renderpass);
	
	/// <summary>
	/// Dessine l'UI par-dessus la scène 3D - Enregistre les commandes de dessin 2D => Doit être appelé pendant un Render Pass actif
	/// </summary>
	/// <param name="commandBuffer">Command buffer a qui on donne les ordre de dessin</param>
	/// <param name="extent">Dimensions de la surface de rendu en 2D, exprimes en pixels (largeur et hauteur)</param>
	void Draw(vk::CommandBuffer commandBuffer, vk::Extent2D extent);

	// Renvoie les demi-dimensions du menu en NDC [-1.0, 1.0].
	UIPushConstants GetPauseMenuSize(uint32_t framebufferWidth,uint32_t framebufferHeight) const;


private : 

	// Creer pipeline graphique rendu UI
	void CreateUIPipeline(vk::RenderPass renderPass);

	// Reference vers contexte vulkan existant
	VulkanContext& m_context;

	// Ressources spécifiques au rendu UI.
	vk::UniquePipelineLayout m_ui_pipelineLayout;
	vk::UniquePipeline m_ui_pipeline;


	////////////////////////
	// Attributs nécessaires pour la texture (ordre des parametres a ne pas TOUCHER !!! => importance dans la memoire)
	
	struct VulkanTexture{

		// Memoire associee a l'image
		vk::UniqueDeviceMemory m_uiImageMemory;

		// Image stockee dans memoire GPU
		vk::UniqueImage m_uiImage;

		// Vue permettant d'accéder a l'image
		vk::UniqueImageView m_uiImageView;

		// Sampler pour definir comment texture est echantillonnee
		vk::UniqueSampler m_uiSampler;

		// Dimensions de la texture
		uint32_t width = 0;
		uint32_t height = 0;
	};

	// Texture pour le menu
	VulkanTexture m_pauseTexture;

	// Descriptor set avec bindings où mettre la texture qui sera passee au fragment shader
	vk::UniqueDescriptorSetLayout m_uiDescriptorSetLayout;
	vk::UniqueDescriptorPool m_uiDescriptorPool;
	vk::DescriptorSet m_uiDescriptorSet;
	
	// Prend le path de la texture et créé texture utilisable par Vulkain 
	VulkanTexture CreateTexture(const std::string& path);

	// Va creer le descriptor set => 1 binding ici seulement pour la texture (pas de lumiere pour l'UI pour le moment)
	void CreateUIDescriptorSet();
	void CreateUIDescriptorSetLayout();

	////////////////////////

};