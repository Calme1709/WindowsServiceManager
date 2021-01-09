import { ServiceType, ServiceStatus, ServiceStartType } from "./index";

export default class WindowsServiceManager {
	/**
	 * Get of all of the services the local machine.
	 * 
	 * @returns The services on the local machine.
	 */
	public static GetServices(): Promise<WindowsServiceManager[]>;

	/**
	 * Get all of the services on the specified machine.
	 * @param machineName - The name of the machine.
	 * 
	 * @returns The services on the specified machine.
	 */
	public static GetServices(machineName: string): Promise<WindowsServiceManager[]>;

	/**
	 * Get all of the device drives on the local machine.
	 * 
	 * @returns The an array of WindowsServiceManagers which are associated with the device drivers on the local machine. 
	 */
	public static GetDevices(): Promise<WindowsServiceManager[]>;

	/**
	 * Get all of the device drives on the specified machine.
	 * 
	 * @param machineName - The name of the machine.
	 * 
	 * @returns The an array of WindowsServiceManagers which are associated with the device drivers on the specified machine. 
	 */
	public static GetDevices(machineName: string): Promise<WindowsServiceManager[]>;

	/**
	 * Create a new WindowsServiceManager that is associated with the specified service on the local machine.
	 * @param serviceName - The service to manage.
	 */
	public constructor(serviceName: string);

	/**
	 * Create a new WindowsServiceManager that is associated with the specified service on the specified machine.
	 * @param serviceName - The service to manage.
	 * @param machineName - The computer on which the service exists.
	 */
	public constructor(serviceName: string, machineName: string);

	/**
	 * A boolean indicating whether the service can be paused and resumed.
	 */
	public readonly CanPauseAndContinue: boolean;

	/**
	 * A boolean indicating whether the service should be notified when the system is shutting down.
	 */
	public readonly CanShutdown: boolean;

	/**
	 * A boolean indicating whether the service can be stopped after it has started.
	 */
	public readonly CanStop: boolean;

	/**
	 * An array of services that depend on the service associated with this WindowsServiceController instance.
	 */
	public readonly DependentServices: WindowsServiceManager[];

	/**
	 * The friendly name of this service, this can be updated on the system by setting this variable.
	 */
	public DisplayName: string;

	/**
	 * The name of the computer on which this service resides.
	 */
	public readonly MachineName: string;
	
	/**
	 * The name of the service which this instance of WindowsServiceManager is associated with.
	 */
	public readonly ServiceName: string;

	/**
	 * All of the services that this
	 */
	public readonly ServicesDependedOn: WindowsServiceManager[];

	/**
	 * The type of service that this WindowsServiceManager is associated with.
	 */
	public readonly ServiceType: ServiceType;

	/**
	 * How the service starts.
	 */
	public readonly StartType: ServiceStartType;

	/**
	 * The current status of the service.
	 */
	public readonly Status: ServiceStatus;

	/**
	 * Disconnect the WindowsServiceManager from the service and free all resources.
	 */
	public Close(): undefined;

	/**
	 * Continue a service after it has been paused.
	 */
	public Continue(): undefined;

	/**
	 * Release the unmanaged resources of the WindowsServiceManager instance and optionally release the managed resources aswell.
	 * @param disposeOfManagedResources - Whether to dispose of the managed resources.
	 */
	public Dispose(disposeOfManagedResources?: boolean): undefined;

	/**
	 * Execute a custom command on the service.
	 * @param command - An application-defined command flag that indicates which custom command to execute. The value must be between 128 and 256, inclusive.
	 */
	public ExecuteCommand(command: number): undefined;

	/**
	 * Suspend a service's operation
	 */
	public Pause(): undefined;

	/**
	 * Start a service passing no arguments.
	 */
	public Start(): undefined;

	/**
	 * Start a service passing the specified arguments.
	 * @param arguments - The arguments to pass to the service when starting it.
	 */
	public Start(arguments: string[]): undefined;

	/**
	 * Wait for a specified status.
	 * @param status - The status to wait for.
	 * 
	 * @returns A promise that resolves when the status is reached, or throws if an error is reached.
	 */
	public WaitForStatus(status: ServiceStatus): Promise<never>;

	/**
	 * Wait for a specified status, if the service does not reach that status within the specified timeout an error will be thrown.
	 * 
	 * @param status - The status to wait for.
	 * @param timeout - The time to wait before throwing an error.
	 * 
	 * @returns A promise that resolves when the service reaches the desired status, or throws when encountering an error or reaching the timeout.
	 */
	public WaitForStatus(status: ServiceStatus, timeout: number): Promise<never>;
}