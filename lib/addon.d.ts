import { ServiceType, ServiceState } from "./index";

interface IServiceInfo {
	/**
	 * The display (friendly) name of the service.
	 */
	displayName: string;

	/**
	 * The name of the service.
	 */
	serviceName: string;

	/**
	 * The current status of the service.
	 */
	status: {
		/**
		 * The type of service this is.
		 */
		serviceType: ServiceType;

		/**
		 * The current state of the service.
		 */
		currentState: ServiceState;

		/**
		 * The controls that the service can accept.
		 */
		acceptsControls: number;
	}
}

export default class NodeJSWindowsServiceManager {
	/**
	 * Get of all of the services on this machine.
	 */
	public static getServices(): Promise<IServiceInfo[]>;

	public constructor(serviceName: string);

	/**
	 * Change the start type of the service. This determines at which point in the computers lifecycle the service will start. Read more here
	 *
	 * @param desiredStartupType - This is the desired start type. This can be any of the following values:
	 * 
	 * Auto - Indicates that the service is to be started (or was started) by the operating system, at system start-up. If an automatically started service depends on a manually started service, the manually started service is also started automatically at system startup.
	 * 
	 * Boot - Indicates that the service is a device driver started by the system loader. This value is valid only for device drivers.
	 * 
	 * Disabled - Indicates that the service is disabled, so that it cannot be started by a user or application.
	 * 
	 * Demand - Indicates that the service is started only manually, by a user (using the Service Control Manager) or by an application.
	 * 
	 * System - Indicates that the service is a device driver started by the IOInitSystem function. This value is valid only for device drivers.
	 * 
	 * @returns This method returns a promise which resolves to whether or not the 
	 */
	public changeStartupType(desiredStartupType: "Auto" | "Boot" | "Disabled" | "Demand" | "System"): Promise<never>;
}